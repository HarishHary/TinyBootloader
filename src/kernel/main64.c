#include <kernel/printf.h>
#include <kernel/main64.h>

void main64() {
  printf("64 BIIIIIIIIIIT\n");
  __asm__ __volatile__ ("hlt");
}
