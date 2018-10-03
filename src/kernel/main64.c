#include <common/asm.h>
#include <common/idt64.h>
#include <kernel/printf.h>
#include <kernel/serial.h>
#include <kernel/main64.h>

extern void win(int (*printf_ptr)(const char *fmt, ...));

void main64() {
  /*
   * Init 64bit idt && Enable 64bit idt
   */
  static idt64 idt64_g[32] = {0};
  init_idt64(idt64_g);
  enable_idt64(idt64_g);

  #ifdef EXTRA_OBJECT
    win(printf);
  #else
    printf("No extra object !\n");
  #endif

  halt();
}
