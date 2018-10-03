CC = gcc
LD = ld

CFLAGS = -std=c99 -Os -fno-pie -ffreestanding -Wall -Werror -fno-stack-protector -fno-stack-protector  
# debug
CFLAGS += -g
# include path settings
CFLAGS +=  -I../../includes \
					 -I../../includes/kernel \
					 -I../../includes/common

ifdef EXTRA_OBJECT
  CFLAGS += -DEXTRA_OBJECT
  EXTRA_OBJECT_PATH = ../../$(EXTRA_OBJECT)
endif

LDFLAGS += -no-pie -Tlinker/kernel.ld --nmagic -nostdlib

ELF = kernel

OBJS32 = 	main32.o \
					cpuid.o  \
					paging.o \
					../common/idt32.o \
					../common/gdt64.o \

OBJS64 = main64.o \
				 printf.o \
				 serial.o \
				 ../common/idt64.o \

KERNEL = kernel.raw

all: $(KERNEL)

$(ELF): CFLAGS += -march=i686 -m32
$(ELF): $(OBJS32)
				for I in $^;									\
				do														\
				objcopy -O elf64-x86-64 $$I;	\
				done
				$(LD) $(LDFLAGS) -o $@ $(OBJS32) $(OBJS64) $(EXTRA_OBJECT_PATH)

$(KERNEL):	$(OBJS64) $(ELF)
						objcopy -O binary $(ELF) $@

clean:
	$(RM) $(ELF) $(KERNEL) $(OBJS32) $(OBJS64)
