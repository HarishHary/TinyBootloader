CC = gcc
LD = ld

CFLAGS = -std=c99 -Os -fno-pie -ffreestanding -march=i686 -m16 -Wall -Werror -fno-stack-protector

# debug
CFLAGS+= -g
# include path settings
CFLAGS+=  -I../../includes \
					-I../../includes/common \
					-I../../includes/bootloader

LDFLAGS = -melf_i386 -no-pie -Tlinker/bootloader.ld --nmagic -nostdlib

ELF = bootloader
BOOTLOADER = bootloader.raw
KERNEL = kernel.raw

KERNEL_SIZE = $(shell stat -c %s ../kernel/$(KERNEL))
CFLAGS += -DKERNEL_SIZE=$(KERNEL_SIZE)

OBJS = stage1.o \
			 stage2.o \
			 mem.o		\
			 drive.o	\
			 ../common/gdt32.o	\

all: $(BOOTLOADER)

$(ELF): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

$(BOOTLOADER): 	$(ELF)
	objcopy -O binary $^ $@

bootloader.o:
	$(CC) $(CFLAGS) -c bootloader.S -o bootloader.o

clean:
	$(RM) $(ELF) $(BOOTLOADER) $(OBJS)
