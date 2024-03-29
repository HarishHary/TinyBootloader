#!/bin/sh

mv ~/.gdbinit.old ~/.gdbinit
make clean
make
qemu-system-i386 -enable-kvm -fda hello64 -nographic -serial stdio -monitor none -s -S -d in_asm,exec 2> instr &
sleep 1
gdb ./src/bootloader/bootloader \
    -ex 'target remote localhost:1234' \
    -ex 'set architecture i8086' \
    -ex 'hbreak *0x7e80' \
    -ex 'continue'
