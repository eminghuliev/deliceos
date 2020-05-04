#ifndef _CPU_H
#define _CPU_H

#include <cstdint>
#define e20_addr 0x14000
namespace x86::cpu {

struct e20_entry {
    uint64_t addr;
    uint64_t len;
    uint32_t type;
    uint32_t res0;
}__attribute__((packed));

static inline void invlpg(void* m)
{
    asm volatile ( "invlpg (%0)" : : "b"(m) : "memory" );
}
}
#endif
