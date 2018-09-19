#include <common/idt.h>
#include <kernel/io.h>

int main()
{
  init_idt();
  outb(MASTER_PORTB, 0xff);
  outb(SLAVE_PORTB, 0xff);
  __asm__ __volatile__("int $0x10\n" "hlt");
  return 0;
}
