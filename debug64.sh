#!/bin/sh

mv ~/.gdbinit ~/.gdbinit.old
make check
qemu-system-x86_64 -fda hello64 -nographic -serial stdio -monitor none -s -S -d in_asm,exec 2> instr &
sleep 1
gdb -ex 'target remote localhost:1234'    \
    -ex 'break *0x100000'                 \
    -ex 'continue'                        \
    -ex 'layout split'                    \
    -ex 'symbol-file ./src/kernel/kernel' \
