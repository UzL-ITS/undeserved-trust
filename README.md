# Installing the Software Stack
This section describes how to setup the software stack. 
Grub and Grub2 are used as a synonym.
Each of the subsection contains two paths: "Do it from scratch" and "use the provided material"
In order to easily install build dependencies for QEMU, OVMF and Grub
you can use `apt-get build-dep <package name>` where package name is the name of the software in the Ubuntu repository.


## Linux Kernel
We implemented our attack directly in the Linux Kernel. The modified source code is provided under
`kernel-src` and .deb packages under `kernel-bin`. In one of the later steps you may need to edit
parts of the attack configuration in the kernel. However in this case you can still use the binary package
to install the kernel and only need to rebuild the kvm module, in which our attack code is located. This will be explained later on.

## QEMU
Execute the `my-build-and-install.sh` or `my-build.sh` script in `qemu`.
Under ubuntu do `sudo apt install build-essential ninja-build git`, enable `deb-src` entries in `/etc/apt/sources.lst`,
`sudo apt update` and finally `sudo apt build-dep qemu-system-x86` to install dependencies.

## Grub
As described in the paper, grub needs to be modified in order to pickup the disk encryption key from a location in memory that is provided to it by OVMF `orig-patches` contains the patches to enable this.
As the grub version coming with ubuntu does not support luks2 we created a version of the code without the luks2 specific changes in `without-luks2-for-ubuntu-grub` .
The plain upstream grub version is missing some parts for booting the Linux OS. Thus we recommend to use the grub source provided by your distribution (ubuntu in our case)

### Build from Scratch
* Download grub2 src package with `apt-get source grub2`
* Copy the files from `without-luks2-for-ubuntu-grub` to the `debian/patches` subfolder
* Add
 ```
0001-SEV-Patch-1-3.patch
0002-SEV-Patch-2-3.patch
0003-AMD-SEV-Patch-3-3.patch
 ```
at the end of `debian/patches/series` to include the patches in the build process
* Test that everything applies correctly with `quilt push -a`
* Build Deb packages with `DEB_BUILD_OPTIONS=nocheck debuild -b -uc -us` (without the nocheck you have endure the horribly slow unit testing)
* Extract the content of the deb package `grub-efi-amd64-bin_<version>.deb` with `dpkg -x <package> <target dir>`. All other .deb packages are not required. We will denote the path to the extracted content as GRUB_DIR and use it when building OVMF

### Using the supplied version
`ubuntu-grub-src` contains the source code that we used in the paper
`ubuntu-grub-bin` contains the resulting binaries. The .deb package is already extracted

The GRUB binary is already bundled in the OVMF binary, so you do not need to do anything here.

## OVMF

Our attack depends on the exact binary layout of the OVMF image. In the attack scenario this is not a problem, as the image is always known to the attacker.
However, our current workflow for adapting the attack to a new OVMF image requires some manual fiddling. In the following we will describe our current workflow. For
reproducing we would advise to use the provided OVMF image, for which we already prepared the attack.

### Supplied Material
`ovmf-src` contains OVMF source code
`OVMF.fd` is the "ready to go" OVMF image for which we prepared the attack.

### Build OVMF from scratch
Note that the code in `ovmf-src` contains the following additional print statement in the function `SecCoreStartupWithStack` at `OvmfPKg/Sec/SecMain.c:920`

```
  DEBUG ((DEBUG_INFO,
    "Vaddr FindAndReportEntryPoints %p\n",
    FindAndReportEntryPoints
    ));
```
We added it to easily get the virtual address of the area where we wanted to place our attack. This could also be achieved by more extensive analysis of the binary.
* Edit `OvmfPkg/AmdSev/Grub/grub.sh` and change the value of of the `-d` parameter of `mkimage` to
`GRUB_DIR/usr/lib/grub/x86_64-efi` (see GRUB_DIR in Grub Section).
* Run `OvmfPkg/AmdSev/Grub/grub.sh` to prepare the grub image for use by OVMF
* Build OVMF using the `build.sh` (top level of OVMF folder). For the next step we will need the resulting OVMF image `./DEBUG_GCC5/FV/OVMF.fd`


## Find OVMF Blocks for Stage 1 attack

### Using a custom OVMF image
Start the OVMF in QEMU and use GDB to break at the address of the targeted area (address given by the print statement of the previous step).

Use the UEFITool [ https://github.com/LongSoft/UEFITool , also available in package sources ] to extract the PE32 image section out of the OVMF image `OVMF.fd`
Use the assembly instructions displayed with GDB to locate the position of
the code in the extracted binary using IDA [ https://www.hex-rays.com/products/ida/support/download_freeware/ ]. This also gives you the binary offset to address at runtime mapping. (this mapping is constant / does not change for different runs).

Next, search for an instruction (in the targeted code area) that starts at a 16-byte aligned address.
This will be the starting point of the attack.

Use the `gomov` tool on the OVMF image to find the blocks required for the gadget displayed in Figure 3 of the paper. You may not find the exact instructions we used. However,
there are many ways to replicate this gadget. Make sure all your blocks are distinct (you cannot use the same block twice, as we do not copy the data but swap the data of two locations).
Also, especially for the cpuid instruction, you may encounter crashes in the next steps if the code blocks you choose are needed to progress to the point of the attack in OVMF binary.

Edit `launch_attack_config` in `kernel-src/arch/x86/kvm/launch-attack.c` in the Linux.
as follows:
* Enter the offsets of your blocks in the OVMF image to `stage1_block_offsets`
* Enter the offset of the targeted location in the OVMF image to `launch_attack_config.target_block`

### Using the supplied image
No actions required.


## Edit Kernel attack code for Stage 2

### Using a custom OVMF image
Use [github.com/JonathanSalwan/ROPgadget] to find the addresses used for the stage 2 from Figure 4. Then edit `launch_attack_config` in `kernel-src/arch/x86/kvm/launch-attack.c`.

For our OVMF binary the address of the shared GHCB page, that we use for leaking the key, is `0x809000`.
If you use another binary this might vary. You could modify the kernel to print the address for you
(in a function with access to an kvm or vcpu/svm struct it's under svm->vmcb->control.ghcb_gpa)

To get the correct timing for manipulating the cpuid values, the attack code needs the number of cpuid calls performed before the execution of the injected cpuid call. Getting this information is a bit messy, as the call count varies between sev-es, plain sev and plain runs of the VM. Thus, we cannot easily use a debugger to get this information. Instead, we added a print statement to the kernels cpuid emulation and edited the OVMF image so that it executes a `hlt` instruction at the point where the cpuid instruction would be. Using the added print statement this gives you the right call count. Enter it in `target_cpuid_call_count`

After editing, rebuild and reload the kvm module with the `my-make-kvm-modules.sh` in the kernel source directory.

### Using the provided OVMF image
No actions required.

## Preparing the disk image for the VM
You can either use the provided image `disk.img` or follow the manual
at [ https://blog.hansenpartnership.com/building-encrypted-images-for-confidential-computing/ ]

# Launching the attack
Almost there :)

To start using the SEV-ES VM you need to prepare something called the launch blob, which contains the public diffie hellmann key of the guest owner.

* Install dependencies for sev-tool: `sudo apt install libssl-dev libvirt-dev uuid-dev`
* Compile sev-tool with: `autoreconf -vif && ./configure && make`
* execute the following commands
  ```
  sudo ./sevtool --pdh_cert_export && sudo ./sevtool --generate_launch_blob 0x5
  cat godh.cert | base64 | sudo tee godh.base64
  cat launch_blob.bin | base64 | sudo tee  launch_blob.base64
  ```
* Edit the Variables at the start of `launch-script.sh` (use the base64 version of the launch blob and the godh) and run it, to start the VM. The VM will wait for
  the secret injection, which will be done in the next step.
* Use `wrapper-sevsecret.sh` to check the launch measurement and provide the secret. You may need to issue `pip3 install Crypto pycryptodome` to install its dependencies. If you are using a custom OVMF image, edit the hash value in the script.
  To calculate it use
 `cat <ovmf image> <vmsa.bin> > outfile.bin ; sha256sum outfile.bin`.  
* If everything worked the VM will print out some lines ending with 
`Vaddr FindAndReportEntryPoints` and halt (observable by the low cpu usage). This means everything worked. To read out the secret from the shared page you can use the QEMU console. Use 
`telnet localhost 5555` to connect and then enter `xp /47bx 0x809130` to dump the content of the shared page to which the secret was leaked. You can check it by getting the hex representation of the launched password via `echo -n <password> | od -A n -t x1` in order to check that it is part of the dump.

You may now do your happy dance
