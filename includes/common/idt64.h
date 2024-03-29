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

void init_idt64(idt64 *idt_g);
void enable_idt64(idt64 *idt_g);
