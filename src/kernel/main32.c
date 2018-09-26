#include <common/gdt.h>
#include <common/idt.h>
#include <kernel/io.h>
#include <kernel/cpuid.h>
#include <kernel/paging.h>
#include <kernel/main64.h>

void main(const u32 kaddr, const u32 klen)
{
    //load 32bit idt && enable 32bit idt
    static idt idt_g[32] = {0};
    init_idt(idt_g);

    //mask irqs
    outb(MASTER_PORTB, 0xff);
    outb(SLAVE_PORTB, 0xff);

    //check CPUID support
    if (!check_support())
      __asm__ __volatile__("hlt");

    //get stack info
    u32 saddr = 0;
    __asm__ __volatile__("mov %%ebp, %0"
                         :"=r"(saddr)
                        );
    saddr = (saddr + 4096 - 1) & ~(4096 - 1);
    u32 slen = 32768;

    //init page tables
    static page pages = {.pml4 = {0}, .pdpt = {0}, .pdt = {0}, .pt = {0}};
    if (!init_page_tables(kaddr, klen, saddr, slen, &pages))
      __asm__ __volatile__("hlt");

    //enable paging
    enable_paging((u32)pages.pml4);

    disable_interrupt();
    //load 64bit gdt && enable 64bit gdt
    //static gdt gdt_g[3] = {0};
    init_gdt();

    //goto 64 main
    //__asm__ __volatile__("int $0x2\n" "hlt");
    main64();
}
