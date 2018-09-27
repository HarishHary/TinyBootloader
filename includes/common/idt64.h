#pragma once

#include "types.h"
#include <kernel/printf.h>
#include <common/asm.h>

typedef struct __attribute__((__packed__)) idt64_r
{
    u16 limit;
    u64 base;
} idt64_r;

typedef struct __attribute__((__packed__)) idt64
{
    u16 offset0_15;    // offset bits 0..15
    u16 selector16_31; // a code segment selector in GDT or LDT
    u8 ist;            // bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
    u8 type_attr;      // type and attributes
    u16 offset16_31;   // offset bits 16..31
    u32 offset32_63;   // offset bits 32..63
    u32 zero;          // reserved
} idt64;

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

void init_idt64(idt64 *idt_g);
void enable_idt64(idt64 *idt_g);
