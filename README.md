# SYSK1 - Hello World 64

This project is a simple bootloader written in C which prints some message to the COM1 serial line.
The source code uses as little assembler as possible.

## Authors

Harish SEGAR <segar_h@epita.fr>

## Requirements

- GNU C Compiler
- GNU Make
- QEMU x86_64 emulator

## Compilation

To compile the project, run the following command on a terminal :

```
make
```

If you want to add, to link a static object file (.o) to the
bootloader, you can do so by running the following command:

```
EXTRA_OBJECT=file.o make
```

This object file must be produced from C code with the
following structure (you can check an example of this in the tests/ folder) :

```c
void win(int (*printf_ptr)(const char *fmt, ...))
{
      /* some code that uses the function in parameter */
}
```

## Execution

You can run the following command to launch the bootloader with QEMU :

```
qemu-system-x86_64 -enable-kvm -fda hello64 -nographic -serial stdio -monitor none
```
