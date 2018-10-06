#include <common/types.h>
#include <common/asm.h>
#include <bootloader/drive.h>
#include <bootloader/stage2.h>

__attribute__((section(".stage1")))
void main(void) {
  static drive drive_g = {0};
  __asm__ __volatile__("mov %%dx, %0"
                       :"=c"(drive_g.drive_id)
                     );

  if (!read_param_drive(&drive_g))
   halt();

  if (!read_section_drive((u32)stage2, 1, 2, &drive_g))
   halt();

  stage2(&drive_g);
}
