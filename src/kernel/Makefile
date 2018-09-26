CC = gcc
LD = ld

# language settings
CFLAGS = -std=c99
# binary settings
CFLAGS+= -Os -fno-pie -ffreestanding
# errors settings
CFLAGS+= -Wall -Werror
# debug
CFLAGS+= -g
# architecture settings
#CFLAGS+= -march=i686 -m32
# path settings
CFLAGS+=  -I../../includes \
					-I../../includes/kernel \
					-I../../includes/common

#LDFLAGS = -melf_i386
LDFLAGS += -no-pie
LDFLAGS += -Tlinker/kernel.ld
LDFLAGS += --nmagic
LDFLAGS += -nostdlib

ELF = kernel

OBJS32 = 	main32.o \
					serial.o \
					cpuid.o  \
					paging.o \
					../common/idt.o \
					../common/gdt.o \

OBJS64 = main64.o \
				 printf.o \

KERNEL = kernel.raw

all: $(KERNEL)

$(ELF): CFLAGS+= -march=i686 -m32
$(ELF): $(OBJS32)
				for I in $^;									\
				do														\
				objcopy -O elf64-x86-64 $$I;	\
				done
				$(LD) $(LDFLAGS) -o $@ $(OBJS32) $(OBJS64)

$(KERNEL):	$(OBJS64) $(ELF)
						objcopy -O binary $(ELF) $@

clean:
	$(RM) $(ELF) $(KERNEL) $(OBJS32) $(OBJS64)
