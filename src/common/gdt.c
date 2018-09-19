#include "../includes/common/gdt.h"

static gdt gdt_g[6] = {0};

void init_gdt() {
    /*
     * Null segment
     */
    init_gdt_segment(0x0, 0x0, 0x0,
                     0x0, 0x0, 0x0,
                     0x0, 0x0, 0x0,
                     0x0, 0x0, 0x0,
                     0x0, &gdt_g[0]);
    /*
     * Kernel code segment
     */
    init_gdt_segment(0xffff, 0x0, 0x0,
                     T_ER, S_CD_DT, R0,
                     0x1, 0xff, 0x0,
                     0x0, DB_32b, 0x1,
                     0x0, &gdt_g[1]);
    /*
     * Kernel data segment
     */
    init_gdt_segment(0xffff, 0x0, 0x0,
                     T_RW, S_CD_DT, R0,
                     0x1, 0xff, 0x0,
                     0x0, DB_32b, 0x1,
                     0x0, &gdt_g[2]);
    /*
     * Userland code segment
     */
    init_gdt_segment(0xffff, 0x0, 0x0,
                     T_ER, S_CD_DT, R3,
                     0x1, 0xff, 0x0,
                     0x0, DB_32b, 0x1,
                     0x0, &gdt_g[3]);
    /*
     * Userland data segment
     */
    init_gdt_segment(0xffff, 0x0, 0x0,
                     T_RW, S_CD_DT, R3,
                     0x1, 0xff, 0x0,
                     0x0, DB_32b, 0x1,
                     0x0, &gdt_g[4]);
    /*
     * TSS segment
     */
    init_gdt_segment(0x0, 0x0, 0x0,
                     0x0, 0x0, 0x0,
                     0x0, 0x0, 0x0,
                     0x0, 0x0, 0x0,
                     0x0, &gdt_g[5]);

    gdt_r gdtr;
    gdtr.base = (u32)gdt_g;
    /* gdt base address */
    gdtr.limit = sizeof(gdt_g) - 1;
    /* gdt size - 1 */
    __asm__ __volatile__("lgdt %0\n"
            :/* no output */
            : "m" (gdtr)
            : "memory");
    __asm__ __volatile__("mov %0, %%ds\n\t"
                "mov %0, %%fs\n\t"
                "mov %0, %%gs\n\t"
                "mov %0, %%ss\n\t"
                "ljmp %1, $next\n\t"
                "next:\n\t"
            :
            :"r" (2 << 3),// index of data
             "i" (1 << 3) // size cs segment
            :"ax"
            );
}

void init_gdt_segment( u16 segment_limit, u16 base_address, u8 base16_23,
                       TYPE type_field, S desc_type, DPL dpl,
                       u8 seg_present, u8 limit16_19, u8 avl,
                       u8 l, DB default_oper, u8 granularity,
                       u8 base24_31, gdt *gdt ) {
    gdt->segment_limit  =   segment_limit & 0xffff;
    gdt->base_address   =   base_address & 0xffff;
    gdt->base16_23      =   (base16_23 >> 4) & 0xff;
    gdt->type_field     =   type_field & 0xf;
    gdt->desc_type      =   desc_type & 0x1;
    gdt->dpl            =   dpl & 0x3;
    gdt->seg_present    =   seg_present & 0x1;
    gdt->limit16_19     =   limit16_19;
    gdt->avl            =   avl & 0x1;
    gdt->l              =   l & 0x1;
    gdt->default_oper   =   default_oper & 0x1;
    gdt->granularity    =   granularity & 0x1;
    gdt->base24_31      =   (base24_31 >> 6) & 0xff;
}
