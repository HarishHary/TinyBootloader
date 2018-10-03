#pragma once

#include "types.h"

/*
 * Segment types
 */
typedef enum TYPE
{
	/* data */
	TYPE_RO    = 0x00,
	TYPE_ROA   = 0x01,
	TYPE_RW    = 0x02,
	TYPE_RWA   = 0x03,
	TYPE_ROED  = 0x04,
	TYPE_ROEDA = 0x05,
	TYPE_RWED  = 0x06,
	TYPE_RWEDA = 0x07,
	/* code */
	TYPE_EO    = 0x08,
	TYPE_EOA   = 0x09,
	TYPE_ER    = 0x0a,
	TYPE_ERA   = 0x0b,
	TYPE_EOC   = 0x0c,
	TYPE_EOCA  = 0x0d,
	TYPE_ERC   = 0x0e,
	TYPE_ERCA  = 0x0f
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
typedef struct __attribute__((__packed__)) gdt32
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
} gdt32;


typedef struct __attribute__((__packed__)) gdt32_r
{
    u16 limit;
    u32 base;
} gdt32_r;

void init_gdt32(gdt32 *gdt32_g);
