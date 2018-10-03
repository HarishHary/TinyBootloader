#include <common/idt32.h>

#define define_print(num) \
    void idt_print_##num()  \
    { \
         printf("Interrupt "#num); \
    } \
    idt_set_gate((u32)&idt_print_##num, 0x8, \
    0x8E, &idt_g[num]);\

static inline void idt_set_gate(u32 offset, u16 ss, u8 flags,
        idt *idt_g);

void init_idt(idt *idt_g)
{
  define_print(0);
  define_print(1);
  define_print(2);
  define_print(3);
  define_print(4);
  define_print(5);
  define_print(6);
  define_print(7);
  define_print(8);
  define_print(9);
  define_print(10);
  define_print(11);
  define_print(12);
  define_print(13);
  define_print(14);
  define_print(15);
  define_print(16);
  define_print(17);
  define_print(18);
  define_print(19);
  define_print(20);
  define_print(21);
  define_print(22);
  define_print(23);
  define_print(24);
  define_print(25);
  define_print(26);
  define_print(27);
  define_print(28);
  define_print(29);
  define_print(30);
  define_print(31);
}

void enable_idt(idt *idt_g)
{
  static idt_r idtr;
  idtr.base = (u32)idt_g;
  /* idt base address */
  idtr.limit = sizeof(idt_g) - 1;
  /* idt size - 1 */
  __asm__ __volatile__ ("lidt %0\n"
                        : /* no output */
                        : "m" (idtr)
                        : "memory");
  enable_interrupt();
}

static inline void idt_set_gate(u32 offset, u16 selector, u8 flags,
    idt *idt_g)
{
  idt_g->offset0_15    = (offset & 0xFFFF);
  idt_g->offset16_31   = (offset >> 16) & 0xFFFF;
  idt_g->ist           = 0;
  idt_g->type_attr     = flags;
  idt_g->selector16_31 = selector;
}
