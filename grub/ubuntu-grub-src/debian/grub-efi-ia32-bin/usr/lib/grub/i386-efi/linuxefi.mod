ELF                      ?      4     (            GNU   ?      ??U??WVS??,?u??uPPh    j???????1???=   ??uPPh   j???????1??E?    ??  PPjV??????E???1?1?????  9???   PPh  ?E?4???????U????????\  G?]??E?    ?P8?@4???? ??????MЉUԋE؋U?E?U?9؉?M????ɉE??E?    ??u;E?t?   ?Å??x???PPh4   j??   P???  ??Pj h??????????   ????uVVhI   j??   ??Ph_   hq   j{hz   ??????    ??  ?5   ??  ?? 1??&?MЍ?؉ƃ?QVj R?U???????U?֋M?A??9?tS??    ?E؋E????M??P4QR?U?VP????????U?9ЋM?t??=     u&V?E?U??4h?   j?????????    ??  1?9?}???E??4??????F???????u???????   ????t ?=     tR???  ??S1?RP????????    ?e?[^_]???U?????5   ??????       ?   ????t#P?    ??  ?  ??P??1?RP????????   ??t#P?    ??8     ??P??1?RP????????   ??tQj 1?RP????????    ??tRj1?RP???????1?????U??????5    ?   1?RP?5   ?????????U??WVS??(?}?5   ????????} uRRh    j???????1???  PPj?7??????E???1ۅ???  ?@4?E???P??????ƃ???uPPh?   ?c  P?u?V?u???????Ã?;E?tP?7h?   j?#???????t)PPSV?????????P?7h?   j???????1??
  Sjj h??????????ã    ????uQQh?   ??  ??Ph  hq   h?   hz   ???????h?  j ?5    ????????  ??  ?????    ???  RP?E?Sh&  hq   h?   hz   ???????S?E?P?    ?  P??????    ?$hG  hq   h?   hz   ??????? hT  hq   h?   hz   ???????f???  U?tRRhl  ??   h?  hq   h?   hz   ??????????  @v	PPh?  ?sh?  hq   h?   hz   ???????f??  w	PPh?  ?Ch?  hq   h  hz   ?????????d   u	PPh?  ?f??6  ??fHuPPh  j?_  h@  hq   h  hz   ?????????8     ??Pj h??????????   ????uPPhT  ?  ??Phk  hq   h'  hz   ???????jh?  ?5   ??????$    ??8  ??P?   ??PW?u??????? h?  hq   h/  hz   ??????   ??(  h?  hq   h2  hz   ???????d  ?   ???  G??	????`  ?  ??P??\  ??X  ??????   ????u$P??`  ?  ??Pj h??????????   ???   ??uQQh?  j?????????   ??Ph?  hq   hD  hz   ???????j hS  h  ??????      X?5   h?  hq   hH  hz   ??????   ??  ???E?)?P?WR?????h	  hq   hM  hz   ?????ƃ  ?? h%  hq   hP  hz   ??????    fǀ&   XZ?5   ?5   hH  hq   hT  hz   ??????? ???u??????????t??V????????=     t???5   ??????       ???   ??t+??t'?=    uV??8     ??P??1?RP????????   ??t?=    uQj 1?RP????????    ??t?=    uRj1?RP????????    ?e?[^_]???U????j hn  j h*  hq   ??????   ??j hz  j h    h?  ??????    ?E?   ?? ????U?????5   ?????X?5    ???????????U??WVS???}ؾ    ?   ??h?  h?  j-h?  ?????Y[j ?E?P??????É$h?  h?  j/h?  ??????? ??u h?  h?  j2h?  ???????1???   h?  h?  j6h?  ?????XZ?u?u??É$h  h?  j8h?  ??????? ??u h4  h?  j;h?  ????????   ? ??ShZ  h?  j@h?  ??????? ????e?[^_]???U??WVS???]?}?u?5    ?????????t?X ?h?  h?  j]h?  ???????PVWSh?  h?  j`h?  ????????V?5    ?5    ?Ӹ&   ?e?[^_]?P?]`F? C??=?݋#filename expected you need to load the kernel first overflow is detected can't allocate initrd initrd_mem = %lx
 linuxefi loader/i386/efi/linux.c premature end of file %s cannot allocate kernel buffer Can't read kernel %s %s has invalid signature cannot allocate kernel parameters params = %p
 copying %zu bytes from %p to %p
 lh is at %p
 checking lh->boot_flag
 invalid magic number checking lh->setup_sects
 too many setup sectors checking lh->version
 kernel too old checking lh->handover_offset
 kernel doesn't support EFI handover kernel doesn't support 32-bit handover setting up cmdline
 can't allocate cmdline linux_cmdline = %lx
 BOOT_IMAGE= setting lh->cmd_line_ptr
 computing handover offset
 can't allocate kernel kernel_mem = %lx
 setting lh->code32_start to %p
 setting lh->type_of_loader
 setting lh->ext_loader_{type,ver}
 kernel_mem: %p handover_offset: %08x
 Load Linux. Load initrd. initrdefi Locating shim protocol
 linuxefi loader/efi/linux.c shim_lock: %p
 secureboot shim not available
 Asking shim to verify kernel signature
 shim_lock->verify(): %d
 Kernel signature verification passed
 Kernel signature verification failed (0x%lx)
 Loaded Image base address could not be set
 linux kernel_addr: %p handover_offset: %p params: %p
   LICENSE=GPLv3+  linux boot linuxefi                                                                            ?  T        	  &                                                            	              
               9              O            V              o              |              ?              ?              ?              ?              ?              ?                                        2             J             Y             e             u             ?             ?             ?  D	      ?             ?  O
  ?     ?             ?             ?                           grub_mod_init grub_mod_fini grub_efi_allocate_pages_max grub_efi_system_table params grub_efi_locate_protocol grub_memmove grub_efi_allocate_fixed grub_dl_ref grub_create_loader_cmdline grub_efi_get_loaded_image grub_errno grub_memset grub_efi_image_handle grub_register_command_prio grub_efi_secure_boot grub_unregister_command grub_file_read grub_malloc grub_file_close grub_file_open grub_real_dprintf grub_calloc grub_linuxefi_secure_validate grub_error grub_efi_linux_boot grub_free grub_dl_unref grub_efi_free_pages grub_loader_set          "  +     6     =   "  X      ?     ?             *    :    ?    F    K    P    \    ?    ?    ?    ?    ?  "  ?          $      (    ?  &  G    _    d  %  j    t    ?    ?  &  ?    ?    ?  &  ?    ?  &  ?    ?  &              $  #  <    A    Q    X  "  m    ?    ?    ?    ?    ?    ?  !  ?    ?  "              (    -    7    <    L    Q    j    {    ?    ?    ?    ?    ?    ?    ?    ?    ?    ?    ?    ?    ?    ?    ?        	            +    2    7    A    F    [    b    g    q    v    ?    ?    ?    ?    ?    ?    ?    ?    ?    ?                    #    (    >    K    S    X    b    g    l    w    |    ?    ?    ?    ?    ?    ?    ?    ?          "      !    +    0    :    ?    D  '  J    U    Z    _    i    n    t    ?    ?    ?    ?    ?    ?    ?    ?    ?    ?    ?    ?    ?    ?    ?    ?          $  $    0    5  %  ;    H    V    t  &  |    ?    ?  &  ?    ?    ?  &  ?    ?    ?    ?    ?    ?    ?    ?    	    	    	    	    *	    /	    6	    ;	    U	    a	    f	    m	    r	    	    ?	    ?	    ?	    ?	    ?	    ?	    ?	    ?	    ?	    ?	    ?	    ?	    ?	    ?	    ?	    ?	    
    
    
    
    ,
    1
    8
    =
    g
    l
    }
    ?
    ?
    ?
    ?
    ?
    ?
    ?
    ?
    ?
     .symtab .strtab .shstrtab .note.gnu.property .rel.text .rodata .rodata.str1.1 .data .module_license .bss .moddeps .modname                                                      4                     2             P   ?
                 .   	   @       ?  h              8                                  @      2       0  ?                O                                  U                                 e             ,  $                  j              ,                    s              7  	                                @  ?              	              ?                                 D  |                  