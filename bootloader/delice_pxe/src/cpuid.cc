#include "misc/cpuid.h"
std::tuple<uint32_t, uint32_t, uint32_t, uint32_t> cpuid(uint32_t eax_ , uint32_t ecx_){
    uint32_t eax, ebx, ecx, edx;
    __asm__ volatile("cpuid"
    : "=a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx)
    : "a" (eax_), "c" (ecx_));
    return {eax, ebx, edx, ecx};
}
