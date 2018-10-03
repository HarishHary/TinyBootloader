#include <common/idt64.h>

#define define_print64(num) \
  void idt_print64_##num()  \
  { \
       printf("Interrupt "#num); \
  } \
  idt_set_gate64((u64)&idt_print64_##num, 0x8, \
  0x8E, &idt_g[num]);\

static inline void idt_set_gate64(u64 offset, u16 selector, u8 flags,
  idt64 *idt_g);

void init_idt64(idt64 *idt_g)
{
  define_print64(0);
  define_print64(1);
  define_print64(2);
  define_print64(3);
  define_print64(4);
  define_print64(5);
  define_print64(6);
  define_print64(7);
  define_print64(8);
  define_print64(9);
  define_print64(10);
  define_print64(11);
  define_print64(12);
  define_print64(13);
  define_print64(14);
  define_print64(15);
  define_print64(16);
  define_print64(17);
  define_print64(18);
  define_print64(19);
  define_print64(20);
  define_print64(21);
  define_print64(22);
  define_print64(23);
  define_print64(24);
  define_print64(25);
  define_print64(26);
  define_print64(27);
  define_print64(28);
  define_print64(29);
  define_print64(30);
  define_print64(31);
}

void enable_idt64(idt64 *idt_g)
{
  static idt64_r idtr64;
  idtr64.base = (u64)idt_g;
  /* idt base address */
  idtr64.limit = (sizeof(idt_g) * 32 * 2) - 1;
  /* idt size - 1 */
  __asm__ __volatile__ ("lidt %0\n"
                        : /* no output */
                        : "m" (idtr64)
                        : "memory");
  enable_interrupt();
}

static inline void idt_set_gate64(u64 offset, u16 selector, u8 flags,
    idt64 *idt_g)
{
  idt_g->offset0_15    = (offset & 0xFFFF);
  idt_g->offset16_31   = (offset >> 16) & 0xFFFF;
  idt_g->ist           = 0;
  idt_g->type_attr     = flags;
  idt_g->selector16_31 = selector;
  idt_g->offset32_63   = (offset >> 32) & 0xFFFFFFFF;
  idt_g->zero          = 0;
}
