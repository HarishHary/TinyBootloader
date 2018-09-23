#include <kernel/cpuid.h>

static int check_cpuid_support()
{
  const u32 ID_BIT = (1 << 21);
  u32 eflags = 0;

  __asm__ __volatile__("pushf\n"
                       "pop %0"
                       :"=r"(eflags)
                      );

  u32 flipped_eflags = (eflags ^ ID_BIT);

  __asm__ __volatile__("push %0\n"
                       "popf"
                       :
                       :"r"(flipped_eflags)
                      );

  __asm__ __volatile__("pushf\n"
                       "pop %0"
                       :"=r"(flipped_eflags)
                      );

  __asm__ __volatile__("push %0\n"
                       "popf"
                       :
                       :"r"(eflags)
                      );

  return eflags != flipped_eflags;
}


int check_support()
{
  const u32 LM_BIT = (1 << 29);
  const u32 FUNC = 0x80000000;
  const u32 EXTENDED_FUNC = 0x80000001;

  if (!check_cpuid_support())
    return 0;

  u32 eax = 0;
  u32 edx = 0;

  __asm__ __volatile__ ("cpuid"
                        : "=a" (eax), "=d" (edx)
                        : "a" (FUNC)
                        : "ebx", "ecx");

  if (eax < EXTENDED_FUNC)
    return 0;

  __asm__ __volatile__ ("cpuid"
                        : "=a" (eax), "=d" (edx)
                        : "a" (EXTENDED_FUNC)
                        : "ebx", "ecx");

  return (edx & LM_BIT) != 0;
}
