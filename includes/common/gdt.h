#pragma once

#include "types.h"

typedef enum gdt_flags {
    READ         = 0x0,
    ACCESS       = 0x1,
    WRITE        = 0x1 << 1,
    EXPAND_DOWN  = 0x1 << 2,
    EXECUTE      = 0x1 << 3,
    CODE_OR_DATA = 0x1 << 4, /* 0x10 */
    RING_1       = 0x1 << 5,
    RING_2       = 0x2 << 5,
    RING_3       = 0x3 << 5
} gdt_flags;

/*
* Segment types
*/
typedef enum TYPE
{
    /* data type */
    T_RO,
    T_ROA,
    T_RW,
    T_RWA,
    T_ROED,
    T_ROEDA,
    T_RWED,
    T_RWEDA,

    /* code type */
    T_EO,
    T_EOA,
    T_ER,
    T_ERA,
    T_EOC,
    T_EOCA,
    T_ERC,
    T_ERCA
} TYPE;

/*
* Descriptor type (0 = system; 1 = code or data)
*/
typedef enum S
{
    S_SYS,
    S_CD_DT,
} S;

/*
* Descriptor privilege level
*/
typedef enum DPL
{
    R0,
    R1,
    R2,
    R3
} DPL;

/*
* Default operation size (0 = 16-bit segment; 1 = 32-bit segment)
*/
typedef enum DB
{
    DB_16b,
    DB_32b,
} DB;

/*
* GDT Main Structure
*/
typedef struct __attribute__((__packed__)) gdt
{
    u16 segment_limit;   /* Segment limit 0:15 */
    u16 base_address;    /* Base address 0:15 */
    u8 base16_23;        /* Base 16:23 */
    TYPE type_field : 4; /* Type - Segment type */
    S  desc_type    : 1; /* S - Descriptor type (0 = system; 1 = code or data) */
    DPL dpl         : 2; /* DPL - Descriptor privilege level */
    u8 seg_present  : 1; /* P - Segment present */
    u8 limit16_19   : 4; /* Seg-Limit 16:19 - Segment Limit */
    u8 avl          : 1; /* AVL - Available for use by system software */
    u8 l            : 1; /* L - 64-bit code segment (IA-32e mode only) */
    DB default_oper : 1; /* D/B - Default operation size (0 = 16-bit segment; 1 = 32-bit segment) */
    u8 granularity  : 1; /* G - Granularity */
    u8 base24_31;        /* Base 24:31 */
} gdt;

typedef struct __attribute__((__packed__)) gdt_r
{
    u16 limit;
    u32 base;
} gdt_r;

void init_gdt();
void init_gdt_segment( u16 segment_limit, u16 base_address, u8 base16_23,
                      TYPE type_field, S desc_type, DPL dpl,
                      u8 seg_present, u8 limit16_19, u8 avl,
                      u8 l, DB default_oper, u8 granularity,
                      u8 base24_31, gdt *gdt );
