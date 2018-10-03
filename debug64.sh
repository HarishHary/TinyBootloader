#!/bin/sh

mv ~/.gdbinit ~/.gdbinit.old
make check
qemu-system-x86_64 -fda hello64 -nographic -serial stdio -monitor none -s -S -d in_asm,exec 2> instr &
sleep 1
gdb -ex 'target remote localhost:1234'    \
    -ex 'break *0x1000c8'                 \
    -ex 'continue'                        \
    -ex 'si'                              \
    -ex 'layout split'                    \
    -ex 'disconnect'                      \
    -ex 'set arch i386:x86-64'            \
    -ex 'target remote localhost:1234'    \
    -ex 'symbol-file ./src/kernel/kernel' \
