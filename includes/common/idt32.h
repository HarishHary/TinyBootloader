#pragma once

#include "types.h"
#include <kernel/printf.h>
#include <common/asm.h>

typedef struct __attribute__((__packed__)) idt_r
{
    u16 limit;
    u32 base;
} idt_r;

typedef struct __attribute__((__packed__)) idt
{
    u16 offset0_15;    // offset bits 0..15
    u16 selector16_31; // a code segment selector in GDT or LDT
    u8 ist;            // bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
    u8 type_attr;      // type and attributes
    u16 offset16_31;   // offset bits 16..31
} idt;

void init_idt(idt *idt_g);
void enable_idt(idt *idt_g);
