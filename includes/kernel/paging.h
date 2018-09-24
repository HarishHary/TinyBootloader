#pragma once

#include <common/types.h>

typedef struct __attribute__((aligned(4096))) __attribute__((__packed__)) page
{
    u64 pml4[512];
    u64 pdpt[512];
    u64 pdt[512];
    u64 pt[512];
} page;

int init_page_tables(u32 kaddr, u32 ksize, u32 saddr, u32 ssize, page *page_g);
void enable_paging(u32 pml4_addr);
int mapping(u32 addr, u32 size, page *page_g, int descending);
