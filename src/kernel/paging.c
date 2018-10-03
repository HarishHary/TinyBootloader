#include <kernel/paging.h>

int init_page_tables(u32 kaddr, u32 ksize, u32 saddr, u32 ssize, page *page_g) {
  // mapping of the kernel
  if (!mapping(kaddr, ksize, page_g, 0))
    return 0;

  //mapping of the stack
  if (!mapping(saddr, ssize, page_g, 1))
    return 0;

  return 1;
}

void enable_paging(u32 pml4_addr) {
  //load P4 to cr3 register (cpu uses this to access the P4 table)
  __asm__ __volatile__("mov %0, %%cr3"
                      :
                      :"r"(pml4_addr | 0x3)
                      );

  //enable PAE-flag in cr4 (Physical Address Extension)
  u32 cr4 = 0;
  __asm__ __volatile__("mov %%cr4, %0"
                       :"=r"(cr4)
                     );
  cr4 = cr4 | (1 << 5);
  __asm__ __volatile__("mov %0, %%cr4"
                      :
                      :"r"(cr4)
                      );

  //set the long mode bit in the EFER MSR (model specific register)
  u32 low = 0;
  u32 high = 0;
  __asm__ __volatile__ ("rdmsr"
                    : "=a" (low), "=d" (high)
                    : "c"(0xC0000080));
  u64 val = (((u64)high << 32) | low);
  val = val | (1 << 8);
  __asm__ __volatile__ ("wrmsr"
                :
                : "a" ((u32)val),
                  "d" ((u32)(val >> 32)),
                  "c" (0xC0000080));

  //enable paging in the cr0 register
  u32 cr0 = 0;
  __asm__ __volatile__("mov %%cr0, %0"
                       :"=r"(cr0)
                     );
  cr0 = cr0 | (1 << 31);
  __asm__ __volatile__("mov %0, %%cr0"
                      :
                      :"r"(cr0)
                      );
}

int mapping(u32 addr, u32 size, page *page_g, int descending)
{
  static u32 PAGE_SIZE  = 4096;
  static u32 PAGE_FLAGS = 0x1 | 0x2 | 0x0; // kernel & read_write & present
  const u32 pages_nb = (size / PAGE_SIZE) + (size % PAGE_SIZE != 0);
  const u32 pt_off   = (addr >> 12) & 0x1ff;
  const u32 pdt_off  = (addr >> 21) & 0x1ff;
  const u32 pdpt_off = (addr >> 30) & 0x1ff;

  if (page_g->pml4[0] == 0)
    page_g->pml4[0] = (u32)(page_g->pdpt) | PAGE_FLAGS;

  if (page_g->pdpt[pdpt_off] == 0)
    page_g->pdpt[pdpt_off] = (u32)(page_g->pdt) | PAGE_FLAGS;

  if (page_g->pdt[pdt_off] == 0)
    page_g->pdt[pdt_off] = (u32)(page_g->pt) | PAGE_FLAGS;

  if (page_g->pt[pt_off] != 0)
    return 0;

  page_g->pt[pt_off] = (u32)addr | PAGE_FLAGS;

  if (descending) {
    for (u32 i = 1; i < pages_nb; i++)
    {
      if (page_g->pt[pt_off - i] != 0)
        return 0;
      page_g->pt[pt_off - i] = page_g->pt[pt_off - i + 1] - PAGE_SIZE;
    }
  } else {
    for (u32 i = 1; i < pages_nb; i++) {
      if (page_g->pt[pt_off + i] != 0)
        return 0;
      page_g->pt[pt_off + i] = page_g->pt[pt_off + i - 1] + PAGE_SIZE;
    }
  }
  return 1;
}
