#pragma once

#include "types.h"
#include "gdt.h"

typedef gdt_r idt_r;

typedef struct __attribute__((__packed__)) idt
{
    u16 offset0_15;
    u16 ss16_31;
    u8  ist        : 3;
    u8  reserved3  : 1;
    u8  reserved4  : 1;
    u8  reserved5_7: 3;
    u8  type8_11   : 4;
    u8  type12     : 1;
    u8  dpl        : 2;
    u8  p          : 1;
    u16 offset16_31;
} idt;

typedef enum idt_flags {
    TASK_GATE      = 0x5,
    INTERRUPT_GATE = 0x6,
    TRAP_GATE      = 0x7,
    GATE_RING_0    = 0x0 << 5,
    GATE_RING_1    = 0x1 << 5,
    GATE_RING_2    = 0x2 << 5,
    GATE_RING_3    = 0x3 << 5,
    GATE_SIZE_32   = 0x1 << 3,
    GATE_SIZE_16   = 0x0 << 3
} idt_flags;

/* Interrupts/Exceptions */
typedef enum idt_traps {
    X86_TRAP_DE = 0,    /*  0, Divide-by-zero */
    X86_TRAP_DB,        /*  1, Debug */
    X86_TRAP_NMI,       /*  2, Non-maskable Interrupt */
    X86_TRAP_BP,        /*  3, Breakpoint */
    X86_TRAP_OF,        /*  4, Overflow */
    X86_TRAP_BR,        /*  5, Bound Range Exceeded */
    X86_TRAP_UD,        /*  6, Invalid Opcode */
    X86_TRAP_NM,        /*  7, Device Not Available */
    X86_TRAP_DF,        /*  8, Double Fault */
    X86_TRAP_OLD_MF,    /*  9, Coprocessor Segment Overrun */
    X86_TRAP_TS,        /* 10, Invalid TSS */
    X86_TRAP_NP,        /* 11, Segment Not Present */
    X86_TRAP_SS,        /* 12, Stack Segment Fault */
    X86_TRAP_GP,        /* 13, General Protection Fault */
    X86_TRAP_PF,        /* 14, Page Fault */
    X86_TRAP_SPURIOUS,  /* 15, Spurious Interrupt */
    X86_TRAP_MF,        /* 16, x87 Floating-Point Exception */
    X86_TRAP_AC,        /* 17, Alignment Check */
    X86_TRAP_MC,        /* 18, Machine Check */
    X86_TRAP_XF,        /* 19, SIMD Floating-Point Exception */
    X86_TRAP_VM,        /* 20, Virtualisation Exception */
    X86_TRAP_IRET = 32, /* 32, IRET Exception */
} idt_traps;

void init_idt(idt *idt_g);
void idt_set_gate(unsigned long offset, unsigned short ss, idt_flags flags,
		  idt *idt_g);
void enable_interrupt(void);
void disable_interrupt(void);
