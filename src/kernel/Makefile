CC = gcc
LD = ld

# language settings
CFLAGS = -std=c99
# binary settings
CFLAGS+= -Os -fno-pie -ffreestanding
# architecture settings
CFLAGS+= -march=i686 -m32
# errors settings
CFLAGS+= -Wall -Werror
# debug
CFLAGS+= -g
# path settings
CFLAGS+=  -I../../includes \
					-I../../includes/kernel \
					-I../../includes/common

LDFLAGS = -melf_i386
LDFLAGS += -no-pie
LDFLAGS += -Tlinker/kernel.ld
LDFLAGS += --nmagic
LDFLAGS += -nostdlib

ELF = kernel

OBJS = 	kernel.o \
				printf.o \
				serial.o \
				../common/idt.o \
				../common/interrupt.o

KERNEL = kernel.raw

all: $(KERNEL)

$(ELF): $(OBJS)
				$(LD) $(LDFLAGS) -o $@ $^

$(KERNEL):	$(ELF)
						objcopy -O binary $^ $@

../common/interrupt.o:
	$(CC) $(CFLAGS) -c ../common/interrupt.S -o ../common/interrupt.o

clean:
	$(RM) $(ELF) $(KERNEL) $(OBJS)
