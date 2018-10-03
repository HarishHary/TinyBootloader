#include <common/types.h>
#include <common/asm.h>
#include <common/gdt32.h>
#include <bootloader/mem.h>
#include <bootloader/drive.h>
#include <bootloader/stage2.h>

static inline u32 nb_sectors(u32 size)
{
  return size / 512 + (size % 512 != 0);
}

__attribute__((section(".stage2")))
void stage2(u16 id) {
  static drive drive_g = {0};
  drive_g.drive_id = id;
  static memory_region region_g = {0};
  if (!detect_upper_mem(&region_g))
    halt();

  u32 kaddr = region_g.base_low;
  u32 klen = KERNEL_SIZE;

  if (!read_param_drive(&drive_g))
    halt();

  if (!read_section_drive(kaddr, 3, nb_sectors(klen), &drive_g))
    halt();

  /*
   * Init 32bit gdt && Enable 32bit gdt
   */
  static gdt32 gdt32_g[3] = {0};
  init_gdt32(gdt32_g);
  static gdt32_r gdtr32;
  gdtr32.base = (u32)gdt32_g;
  gdtr32.limit = sizeof(gdt32_g) - 1;
  __asm__ __volatile__("lgdt %0\n"
                       :/* no output */
                       : "m" (gdtr32)
                       : "memory"
                      );

  __asm__ __volatile__("mov %cr0, %eax\n"
                       "orl $0x1, %eax\n"
                       "cli\n"
                       "mov %eax, %cr0"
                      );

  __asm__ __volatile__("mov %0, %%ds\n\t"
                       "mov %0, %%es\n\t"
                       "mov %0, %%fs\n\t"
                       "mov %0, %%gs\n\t"
                       "mov %0, %%ss\n\t"
                       "ljmp %1, $next\n\t"
                       "next:\n\t"
                       :/* no output */
                       :"r" (2 << 3),// index of data
                        "i" (1 << 3) // size cs segment
                      );
  __asm__ __volatile__("jmp *%0"
                       :
                       :"r"(kaddr)
                      );
}
