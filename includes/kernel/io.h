#pragma once

#include <common/types.h>

#define MASTER_PORTB 0x21
#define SLAVE_PORTB  0xA1

static inline void outb(u16 port, u8 val)
{
	__asm__ __volatile__("outb %0, %1\n\t"
			                : /* No output */
			                : "a" (val), "d" (port));
}

static inline u8 inb(u16 port)
{
	u8 res;
	__asm__ __volatile__("inb %1, %0\n\t"
			                : "=&a" (res)
			                : "d" (port));
	return res;
}
