#include <common/idt.h>
#include <kernel/serial.h>

static idt idt_g[256];

void init_idt(void)
{
    idt_set_gate((unsigned long)isr0, 0x8,
        TRAP_GATE | GATE_RING_0 | GATE_SIZE_32, &idt_g[0]);
    idt_set_gate((unsigned long)isr1, 0x8,
        TRAP_GATE | GATE_RING_0 | GATE_SIZE_32, &idt_g[1]);
    idt_set_gate((unsigned long)isr2, 0x8,
        TRAP_GATE | GATE_RING_0 | GATE_SIZE_32, &idt_g[2]);
    idt_set_gate((unsigned long)isr3, 0x8,
        TRAP_GATE | GATE_RING_1 | GATE_SIZE_32, &idt_g[3]);
    idt_set_gate((unsigned long)isr4, 0x8,
        TRAP_GATE | GATE_RING_0 | GATE_SIZE_32, &idt_g[4]);
    idt_set_gate((unsigned long)isr5, 0x8,
        TRAP_GATE | GATE_RING_1 | GATE_SIZE_32, &idt_g[5]);
    idt_set_gate((unsigned long)isr6, 0x8,
        TRAP_GATE | GATE_RING_0 | GATE_SIZE_32, &idt_g[6]);
    idt_set_gate((unsigned long)isr7, 0x8,
        TRAP_GATE | GATE_RING_0 | GATE_SIZE_32, &idt_g[7]);
    idt_set_gate((unsigned long)isr8, 0x8,
        TRAP_GATE | GATE_RING_0 | GATE_SIZE_32, &idt_g[8]);
    idt_set_gate((unsigned long)isr9, 0x8,
        TRAP_GATE | GATE_RING_0 | GATE_SIZE_32, &idt_g[9]);
    idt_set_gate((unsigned long)isr10, 0x8,
        TRAP_GATE | GATE_RING_0 | GATE_SIZE_32, &idt_g[10]);
    idt_set_gate((unsigned long)isr11, 0x8,
        TRAP_GATE | GATE_RING_0 | GATE_SIZE_32, &idt_g[11]);
    idt_set_gate((unsigned long)isr12, 0x8,
        TRAP_GATE | GATE_RING_0 | GATE_SIZE_32, &idt_g[12]);
    idt_set_gate((unsigned long)isr13, 0x8,
        TRAP_GATE | GATE_RING_0 | GATE_SIZE_32, &idt_g[13]);
    idt_set_gate((unsigned long)isr14, 0x8,
        TRAP_GATE | GATE_RING_0 | GATE_SIZE_32, &idt_g[14]);
    idt_set_gate((unsigned long)isr15, 0x8,
        INTERRUPT_GATE | GATE_RING_0 | GATE_SIZE_32, &idt_g[15]);
    idt_set_gate((unsigned long)isr16, 0x8,
        TRAP_GATE | GATE_RING_0 | GATE_SIZE_32, &idt_g[16]);
    idt_set_gate((unsigned long)isr17, 0x8,
        TRAP_GATE | GATE_RING_0 | GATE_SIZE_32, &idt_g[17]);
    idt_set_gate((unsigned long)isr18, 0x8,
        TRAP_GATE | GATE_RING_0 | GATE_SIZE_32, &idt_g[18]);
    idt_set_gate((unsigned long)isr19, 0x8,
        TRAP_GATE | GATE_RING_0 | GATE_SIZE_32, &idt_g[19]);
    idt_set_gate((unsigned long)isr20, 0x8,
        TRAP_GATE | GATE_RING_0 | GATE_SIZE_32, &idt_g[20]);
    idt_set_gate((unsigned long)isr21, 0x8,
        INTERRUPT_GATE | GATE_RING_0 | GATE_SIZE_32, &idt_g[21]);
    idt_set_gate((unsigned long)isr22, 0x8,
        INTERRUPT_GATE | GATE_RING_0 | GATE_SIZE_32, &idt_g[22]);
    idt_set_gate((unsigned long)isr23, 0x8,
        INTERRUPT_GATE | GATE_RING_0 | GATE_SIZE_32, &idt_g[23]);
    idt_set_gate((unsigned long)isr24, 0x8,
        INTERRUPT_GATE | GATE_RING_0 | GATE_SIZE_32, &idt_g[24]);
    idt_set_gate((unsigned long)isr25, 0x8,
        INTERRUPT_GATE | GATE_RING_0 | GATE_SIZE_32, &idt_g[25]);
    idt_set_gate((unsigned long)isr26, 0x8,
        INTERRUPT_GATE | GATE_RING_0 | GATE_SIZE_32, &idt_g[26]);
    idt_set_gate((unsigned long)isr27, 0x8,
        INTERRUPT_GATE | GATE_RING_0 | GATE_SIZE_32, &idt_g[27]);
    idt_set_gate((unsigned long)isr28, 0x8,
        INTERRUPT_GATE | GATE_RING_0 | GATE_SIZE_32, &idt_g[28]);
    idt_set_gate((unsigned long)isr29, 0x8,
        INTERRUPT_GATE | GATE_RING_0 | GATE_SIZE_32, &idt_g[29]);
    idt_set_gate((unsigned long)isr30, 0x8,
        INTERRUPT_GATE | GATE_RING_0 | GATE_SIZE_32, &idt_g[30]);
    idt_set_gate((unsigned long)isr31, 0x8,
        INTERRUPT_GATE | GATE_RING_0 | GATE_SIZE_32, &idt_g[31]);

    idt_r idtr;
    idtr.base = (u32)idt_g;
    /* idt base address */
    idtr.limit = sizeof(idt_g) - 1;
    /* idt size - 1 */
    __asm__ __volatile__("lidt %0\n"
        :
        /* no output */
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

void idt_print(u32 num)
{
    write("Interrupt X", 12);
}
