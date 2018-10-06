#include <bootloader/drive.h>

int read_param_drive(drive *drive_g) {
  u8 error = 0;
  __asm__ __volatile__ ("int $0x13\n"
                        :"=c" (drive_g->spt),
                         "=d" (drive_g->heads)
                        :"a"(0x08 << 8 | 0x0),
                         "d"(drive_g->drive_id >> 8),
                         "D"(0)
                        :"%ebx"
                       );

  __asm__ __volatile__("setc %b0"
                       :"=r" (error)
                      );

  if (!error)
  {
    drive_g->spt   = (u16)(drive_g->spt & 0x3f);
    drive_g->heads = (u16)(drive_g->heads >> 8);
  }
  return !error;
}

int read_section_drive(u32 addr, u16 lba, u32 nb_sectors, drive *drive_g) {
  u8 error = 0;
  u8 al = 0;
  const u32 cylinder = lba / (drive_g->heads * drive_g->spt);
  const u32 head   = (lba / drive_g->spt) % drive_g->heads;
  const u32 sector = (lba % drive_g->spt) + 1;

  __asm__ __volatile__("mov %0, %%es"
                       :
                       :"r"((u16)(addr < 0x100000 ? (addr >> 4) & 0xF000 : 0xFFFF))
                      );

  __asm__ __volatile__("int $0x13"
                       :"=a"(al)
                       :"a"((u16)(0x02 << 8 | nb_sectors)),
                        "b"((u16)(addr < 0x100000 ? addr & 0xFFFF : (addr & 0xFFFF) + 0x10)),
                        "c"((u16)((cylinder << 8) | sector)),
                        "d"((u16)((head << 8) | drive_g->drive_id))
                      );

  __asm__ __volatile__("setc %b0"
                       :"=r" (error)
                      );

  return !error && al == nb_sectors;
}
