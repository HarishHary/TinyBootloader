#include <common/idt.h>
#include <kernel/serial.h>

#define define_print(num) \
    void idt_print_##num()  \
    { \
         write("Interrupt "#num, 15); \
    } \
    idt_set_gate((unsigned long)&idt_print_##num, 0x8, \
    TRAP_GATE | GATE_RING_0 | GATE_SIZE_32, &idt_g[num]);\

void init_idt(void)
{
    static idt idt_g[32];
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
    idt_r idtr;
    idtr.base = (u32)idt_g;
    /* idt base address */
    idtr.limit = sizeof(idt_g) - 1;
    /* idt size - 1 */
    __asm__ __volatile__("lidt %0\n"
        : /* no output */
        : "m" (idtr)
        : "memory");
    __asm__ __volatile__("sti\n");
}

void idt_set_gate(unsigned long offset, unsigned short ss, idt_flags flags,
    idt *idt_g)
{
    idt_g->offset0_15   = (offset & 0xFFFF);
    idt_g->offset16_31  = (offset >> 16) & 0xFFFF;
    idt_g->ss16_31      = ss;
    idt_g->type8_11     = flags & 0x1F;
    idt_g->dpl          = flags >> 5;
    idt_g->p            = 1;
}
