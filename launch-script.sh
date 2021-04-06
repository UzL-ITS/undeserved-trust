#!/bin/bash
#path to ovmf binary (with grub included)
OVMF_PATH="/home/luca/sev-tools-with-secret-patches/artifact-eval/OVMF.fd"
#launch blob generated with sevtool
SESSION_FILE="/home/luca/sev-tools-with-secret-patches/sev-tool/launch_blob.base64"
#godh.cert generated with sevtool
DH_CERT_FILE="/home/luca/sev-tools-with-secret-patches/sev-tool/godh.base64"
#path to disk image
DISK_IMAGE="/home/luca/kvm-development/vms/disk.img"
#path qemu executeable (qemu-system-x86_64)
/home/luca/sev-tools-with-secret-patches/eval-test/qemu-bin/bin/qemu-system-x86_64 \
\
-enable-kvm \
-cpu EPYC \
-machine q35 \
-smp 1,maxcpus=64 \
-m 5000M,slots=5,maxmem=30G -no-reboot \
-drive if=pflash,format=raw,unit=0,file="$OVMF_PATH",readonly \
-netdev user,id=vmnic -device e1000,netdev=vmnic,romfile=  \
-drive file="$DISK_IMAGE",if=none,id=disk0,format=raw \
-device virtio-scsi-pci,id=scsi0,disable-legacy=on,iommu_platform=true \
-device scsi-hd,drive=disk0 \
-machine memory-encryption=sev0,vmport=off \
-object sev-guest,id=sev0,policy=0x5,cbitpos=47,reduced-phys-bits=1,session-file="$SESSION_FILE",dh-cert-file="$DH_CERT_FILE" \
-nographic -monitor pty -monitor unix:monitor,server,nowait \
-device virtio-rng-pci,disable-legacy=on,iommu_platform=true  -net nic,model=rtl8139 \
-net user,hostfwd=tcp::7777-:22   \
-monitor telnet:127.0.0.1:5555,server,nowait   \
-qmp tcp::5550,server,nowait   \
-name my-vm,debug-threads=on   \
-S
