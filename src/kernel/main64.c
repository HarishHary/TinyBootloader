#include <common/asm.h>
#include <common/idt64.h>
#include <kernel/printf.h>
#include <kernel/serial.h>
#include <kernel/main64.h>

extern void win(int (*printf_ptr)(const char *fmt, ...));

void main64() {
  /*
   * Init 32bit idt && Enable 32bit idt
   */
  static idt64 idt64_g[32] = {0};
  init_idt64(idt64_g);
  enable_idt64(idt64_g);

  printf("Hello World !\n");
  __asm__ __volatile__("int $0x10");
  idle();
}
