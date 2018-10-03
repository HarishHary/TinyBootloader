#pragma once

#include <common/types.h>

typedef struct __attribute__((__packed__)) memory_region
{
  u32 base_low;
  u32 base_high;
  u32 len_low;
  u32 len_high;
  u32 type;
  u32 acpi;
} memory_region;

int detect_upper_mem(memory_region *region_g);
