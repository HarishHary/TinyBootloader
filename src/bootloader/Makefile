CC = gcc
LD = ld

# language settings
CFLAGS = -std=c99
# binary settings
CFLAGS+= -Os -fno-pie -ffreestanding
# architecture settings
CFLAGS+= -march=i686 -m16
# errors settings
CFLAGS+= -Wall -Werror
# debug
# CFLAGS+= -g
# path settings
CFLAGS+=  -I../../includes\
					-I../../includes/bootloader

LDFLAGS = -melf_i386
LDFLAGS += -no-pie
LDFLAGS += -Tlinker/bootloader.ld
LDFLAGS += --nmagic
LDFLAGS += -nostdlib

ELF = bootloader
BOOTLOADER = bootloader.raw
KERNEL = kernel.raw

KERNEL_SIZE = $(shell stat -c %s ../kernel/$(KERNEL))
CFLAGS += -DKERNEL_SIZE=$(KERNEL_SIZE)

OBJS = bootloader.o

all: $(BOOTLOADER)

$(ELF): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

$(BOOTLOADER): 	$(ELF)
	objcopy -O binary $^ $@

bootloader.o:
	$(CC) $(CFLAGS) -c bootloader.S -o bootloader.o

clean:
	$(RM) $(ELF) $(BOOTLOADER) $(OBJS)
