#ifndef _CPUID_H
#define _CPUID_H
#include <tuple>
std::tuple<uint32_t, uint32_t, uint32_t, uint32_t> 
           cpuid(uint32_t eax_, uint32_t ecx_); 
#endif
