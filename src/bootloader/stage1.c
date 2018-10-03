#include <common/types.h>
#include <common/asm.h>
#include <bootloader/stage2.h>

__attribute__((section(".stage1")))
void main(void) {
  volatile static u16 drive_id = 0;
  volatile static u16 spt = 0;
  volatile static u16 heads = 0;

  __asm__ __volatile__("mov %%dx, %0"
                       :"=c"(drive_id)
                     );

  u8 error = 0;
  volatile static u16 ret_cx = 0;
  volatile static u16 ret_dx = 0;

  __asm__ __volatile__ ("mov $0, %%ax\n"
                        "mov %%ax, %%es\n"
                        "mov $0, %%di\n"
                        "mov $0x08, %%ah\n"
                        "mov %0, %%dx\n"
                        "int $0x13\n"
                        :"=c" (ret_cx),
                         "=d" (ret_dx)
                        :"c"(drive_id)
                        );

  __asm__ __volatile__("setc %b0"
                       :"=r" (error)
                      );

  if (error)
    halt();

  error = 0;
  u8 al = 0;
  spt = ret_cx & 0x3f;
  heads = ret_dx >> 8;
  volatile static u32 selector = 0;
  volatile static u32 offset = 0;

  volatile const u32 cylinder = 1 / (heads * spt);
  volatile const u32 head   = (1 / spt) % heads;
  volatile const u32 sector = (1 % spt) + 1;

  if ((u32)stage2 < (1 << 20)) {
    selector = ((u32)stage2  >> 4) & 0xf000;
    offset = (u32)stage2 & 0xffff;
  } else {
    selector = 0xffff;
    offset = ((u32)stage2 & 0xffff) + 0x10;
  }

  volatile const u16 ax = (u16)(0x2 << 8 | 2);
  volatile const u16 bx = (u16)offset;
  volatile const u16 cx = (u16)((cylinder  << 8) | sector);
  volatile const u16 dx = (u16)((head << 8) | drive_id);

  __asm__ __volatile__("mov %0, %%es"
                       :
                       :"r"((u16)selector)
                      );

  __asm__ __volatile__("int $0x13"
                       :
                       :"a"(ax),
                        "b"(bx),
                        "c"(cx),
                        "d"(dx)
                      );

  __asm__ __volatile__(""
                       :"=a"(al)
                      );

  __asm__ __volatile__("setc %b0"
                       :"=r"(error)
                      );

  if (error && al != 2)
    halt();

  stage2(drive_id);
}
