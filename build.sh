#!/usr/bin/env bash

rm -rf infinit disk disk.img loopdev
gcc -std=c99 -Wall -Wextra -Werror -O2 -static src/**.c -o infinit

dd if=/dev/zero of=disk.img bs=1M count=64
parted -s disk.img mklabel msdos
parted -s disk.img mkpart primary 2048s 100%
sudo losetup -Pf disk.img --show > loopdev
sudo mkfs.ext4 `cat loopdev`p1
mkdir disk
sudo mount `cat loopdev`p1 disk
sudo mkdir disk/{dev,proc,sys}
sudo cp infinit ./disk
sudo umount disk
sudo losetup -d `cat loopdev`
rm -rf disk loopdev

qemu-system-x86_64 -m 512M -M q35 -kernel vmlinuz-5.16.13 -append "root=/dev/sda1 init=infinit selinux=0" -hda disk.img
