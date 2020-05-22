#include <cstdint>
namespace PerfCounter {
static inline __attribute__((always_inline)) uint64_t read(uint32_t id){
    uint32_t low, high;
    asm volatile ("rdpmc": "=d"(high), "=a"(low): "c"(id) : "memory");
    cpu::mfence();
    cpu::lfence();
    return ((uint64_t)high << 32) | (uint64_t)low;
}
}
