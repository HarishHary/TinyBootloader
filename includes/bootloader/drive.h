#pragma once

#include <common/types.h>

typedef struct __attribute__((__packed__)) drive
{
  u16 drive_id;
  u16 spt;
  u16 heads;
} drive;

int read_param_drive(drive *drive_g);
int read_section_drive(u32 addr, u16 lba, u32 nb_sectors, drive *drive_g);
