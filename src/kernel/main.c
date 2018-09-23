#include <common/idt.h>
#include <kernel/io.h>
#include <kernel/cpuid.h>
#include <kernel/paging.h>

void main(const u32 kaddr, const u32 klen)
{
    //enable idt
    init_idt();

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
    u32 slen = 8192;

    //init page tables
    page pages = { .pml4 = {0}, .pdpt = {0}, .pdt = {0}, .pt = {0}};
    if (!init_page_tables(kaddr, klen, saddr, slen, &pages))
      __asm__ __volatile__("hlt");

    //enable paging
    enable_paging((unsigned long)pages.pml4);

    //enable 64 gdt

    //load 64 gdt

    //goto 64 main
    __asm__ __volatile__("int $0x2\n" "hlt");
}
