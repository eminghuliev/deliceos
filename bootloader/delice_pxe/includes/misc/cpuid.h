#ifndef _CPUID_H
#define _CPUID_H
#define cpuid(eax, ebx, ecx, edx) {\
    __asm__ volatile("cpuid;" \
                     :"=a"((uint32_t)(eax)), "=b"((uint32_t)(ebx)), \
                      "=c"((uint32_t)(ecx)), "=d"((uint32_t)(edx)) \
                     :"a"((uint32_t)(eax)) \
                     :); \
}
#endif
