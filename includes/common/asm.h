#pragma once

static inline void enable_interrupt(void)
{
	__asm__ __volatile__("sti");
}

static inline void disable_interrupt(void)
{
	__asm__ __volatile__("cli");
}

static inline void halt(void)
{
	__asm__ __volatile__("cli\n"
                       "hlt"
                      );
}

static inline void idle(void)
{
	__asm__ __volatile__("jmp .\n");
}
