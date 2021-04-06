#include <grub/err.h>
#include <grub/command.h>
#include <grub/misc.h>
#include <grub/cryptodisk.h>
#include <grub/efi/efi.h>
#include <grub/efi/api.h>
#include <grub/dl.h>

GRUB_MOD_LICENSE ("GPLv3+");

static grub_efi_packed_guid_t sevsecret_guid = GRUB_EFI_SEVSECRET_TABLE_GUID;
static grub_efi_packed_guid_t tableheader_guid = GRUB_EFI_SEVSECRET_TABLE_HEADER_GUID;
static grub_efi_packed_guid_t diskpasswd_guid = GRUB_EFI_DISKPASSWD_GUID;

/*
 * EFI places the secret in the lower 4GB, so it uses a UINT32
 * for the pointer which we have to transform to the correct type
 */
struct sev_secret {
  grub_uint32_t base;
  grub_uint32_t size;
};

struct secret_header {
  grub_efi_packed_guid_t guid;
  grub_uint32_t len;
};

struct secret_entry {
  grub_efi_packed_guid_t guid;
  grub_uint32_t len;
  char data[0];
};

static grub_err_t
grub_efi_sevsecret_find (struct sev_secret *s)
{
  int len;
  struct secret_header *h;
  struct secret_entry *e;
  unsigned char *ptr = (unsigned char *)(unsigned long)s->base;
  
  grub_printf_ (N_("Address of sev_secret struct base %p size %u"),ptr, s->size );
  /* the area must be big enough for a guid and a u32 length */
  if (s->size < sizeof (*h)) {
    grub_printf_ (N_("SEV secret area is too small, but let's ignore that for now"));
    //return grub_error (GRUB_ERR_BAD_ARGUMENT, "SEV secret area is too small, expected at least %d bytes, got %d bytes",sizeof(*h),s->size);
  }
    

  h = (struct secret_header *)ptr;
  if (grub_memcmp(&h->guid, &tableheader_guid, sizeof (h->guid)))
    return grub_error (GRUB_ERR_BAD_ARGUMENT, "SEV secret area does not start with correct guid\n");
  if (h->len < sizeof (*h))
    return grub_error (GRUB_ERR_BAD_ARGUMENT, "SEV secret area is too small (header check)\n");

  len = h->len - sizeof (*h);
  ptr += sizeof (*h);

  while (len >= (int)sizeof (*e)) {
    e = (struct secret_entry *)ptr;
    if (e->len < sizeof(*e) || e->len > (unsigned int)len)
      return grub_error (GRUB_ERR_BAD_ARGUMENT, "SEV secret area is corrupt\n");

    if (! grub_memcmp (&e->guid, &diskpasswd_guid, sizeof (e->guid))) {
      int end = e->len - sizeof(*e);

      /*
       * the passphrase must be a zero terminated string because the
       * password routines call grub_strlen () to find its size
       */
      if (e->data[end - 1] != '\0')
       return grub_error (GRUB_ERR_BAD_ARGUMENT, "SEV secret area disk encryption password is not zero terminated\n");

      return grub_cryptodisk_set_secret (e->data);
    }
    ptr += e->len;
    len -= e->len;
  }
  return grub_error (GRUB_ERR_BAD_ARGUMENT, "SEV secret aread does not contain disk decryption password\n");
}

static grub_err_t
grub_efi_sevsecret_init (void)
{
  unsigned int i;

  for (i = 0; i < grub_efi_system_table->num_table_entries; i++)
    {
      grub_efi_packed_guid_t *guid =
       &grub_efi_system_table->configuration_table[i].vendor_guid;

      if (! grub_memcmp (guid, &sevsecret_guid, sizeof (grub_efi_packed_guid_t))) {
       struct sev_secret *s =
         grub_efi_system_table->configuration_table[i].vendor_table;

       return grub_efi_sevsecret_find(s);
      }
    }
  return grub_error (GRUB_ERR_BAD_ARGUMENT, "No SEV secret found in the EFI configuration table");
}

static grub_err_t
grub_cmd_sevsecret (grub_command_t cmd __attribute__ ((unused)),
                   int argc __attribute__ ((unused)),
                   char **args __attribute__ ((unused)))
{
  return grub_efi_sevsecret_init();
}

static grub_command_t cmd;

GRUB_MOD_INIT(sevsecret)
{
  cmd = grub_register_command ("sevsecret", grub_cmd_sevsecret, 0,
                              N_("Register a SEV secret with cryptomount if one exists"));
}

GRUB_MOD_FINI(sevsecret)
{
  grub_unregister_command (cmd);
}
