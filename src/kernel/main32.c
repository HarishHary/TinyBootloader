#include <common/idt32.h>
#include <common/gdt64.h>
#include <common/asm.h>
#include <kernel/io.h>
#include <kernel/cpuid.h>
#include <kernel/paging.h>
#include <kernel/main64.h>

__attribute__((regparm(2)))
void main(const u32 kaddr, const u32 klen)
{
  /*
   * Get parameters from real mode FIXME still need to understand
   * why I can't use it directly
   */
  volatile static u32 kkaddr = 0;
  volatile static u32 kklen = 0;
  __asm__ __volatile__(""
                       :"=b"(kkaddr),
                        "=d"(kklen)
                      );
  /*
   * Init 32bit idt && Enable 32bit idt
   */
  static idt idt_g[32] = {0};
  init_idt(idt_g);
  enable_idt(idt_g);

  /*
   * Mask irqs
   */
  outb(MASTER_PORTB, 0xff);
  outb(SLAVE_PORTB, 0xff);

  /*
   * Check CPUID support
   */
  if (!check_support())
    halt();

  /*
   * Get stack information
   */
  u32 saddr = 0;
  __asm__ __volatile__("mov %%ebp, %0"
                       :"=r"(saddr)
                      );
  saddr = (saddr + 4096 - 1) & ~(4096 - 1);
  u32 slen = 32768;

  /*
   * Init page tables && Enable paging
   */
  static page pages = {.pml4 = {0}, .pdpt = {0}, .pdt = {0}, .pt = {0}};
  if (!init_page_tables(kkaddr, kklen, saddr, slen, &pages))
    halt();
  enable_paging((u32)pages.pml4);

  /*
   * Disable interrupts
   */
  disable_interrupt();

  /*
   * Init 64bit gdt && Enable 64bit gdt
   */
  static gdt gdt_g[3] = {0};
  init_gdt(gdt_g);
  static gdt_r gdtr;
  gdtr.base = (u32)gdt_g;
  gdtr.limit = sizeof(gdt_g) - 1;
  __asm__ __volatile__("lgdt %0\n"
                       :/* no output */
                       : "m" (gdtr)
                       : "memory"
                      );
  __asm__ __volatile__("mov %0, %%ds\n\t"
                       "mov %0, %%es\n\t"
                       "mov %0, %%fs\n\t"
                       "mov %0, %%gs\n\t"
                       "mov %0, %%ss\n\t"
                       "ljmp %1, $next\n\t"
                       "next:\n\t"
                       :/* no output */
                       :"r" (2 << 3),// index of data
                        "i" (1 << 3) // size cs segment
                      );
  /*
   * Goto 64 main
   */
  main64();
}
