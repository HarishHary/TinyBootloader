#include <bootloader/mem.h>

int detect_upper_mem(memory_region *region_g) {

  u32 signature = 0;
  u32 ret       = 0;
  u8 esdi_sz   = 0;

  do
  {
    __asm__ __volatile__ ("int $0x15"
                          : "=a" (signature),
                            "=b" (ret),
                            "=c" (esdi_sz)
                          : "a"(0xE820),
                            "b"(ret),
                            "c"(sizeof(memory_region)),
                            "d"(0x0534D4150),
                            "D"(region_g)
                          : "memory"
                          );
    if (esdi_sz <= 20 || (region_g->acpi & 1))
    {
      if (region_g->base_low > 0xffff
        && region_g->type == 1
        && region_g->len_low >= KERNEL_SIZE) {
          return 1;
      }
    }
  } while (signature == 0x0534D4150 && ret);

  return 0;
}
