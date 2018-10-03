#include <common/gdt64.h>

static inline void init_gdt_segment(u16 segment_limit, u16 base_address, u8 base16_23,
                       TYPE type_field, S desc_type, DPL dpl,
                       u8 seg_present, u8 limit16_19, u8 avl,
                       u8 l, DB default_oper, u8 granularity,
                       u8 base24_31, gdt *gdt);

void init_gdt(gdt *gdt_g) {
  //Null segment
  init_gdt_segment(0x0, 0x0, 0x0,
                   0x0, 0x0, 0x0,
                   0x0, 0x0, 0x0,
                   0x0, 0x0, 0x0,
                   0x0, &gdt_g[0]);

  // Kernel code segment
  init_gdt_segment(0xffff, 0x0, 0x0,
                   TYPE_ER, S_CD_DT, R0,
                   0x1, 0xff, 0x0,
                   0x1, 0x0, 0x1,
                   0x0, &gdt_g[1]);

  // Kernel data segment
  init_gdt_segment(0xffff, 0x0, 0x0,
                   TYPE_RW, S_CD_DT, R0,
                   0x1, 0xff, 0x0,
                   0x1, 0x0, 0x1,
                   0x0, &gdt_g[2]);
}

static inline void init_gdt_segment(u16 segment_limit, u16 base_address, u8 base16_23,
                       TYPE type_field, S desc_type, DPL dpl,
                       u8 seg_present, u8 limit16_19, u8 avl,
                       u8 l, DB default_oper, u8 granularity,
                       u8 base24_31, gdt *gdt) {
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
