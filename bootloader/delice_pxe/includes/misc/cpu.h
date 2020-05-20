#ifndef _CPU_H
#define _CPU_H
#include <cstddef>
#include <cstdint>
#define e20_addr 0x14000
namespace cpu {

struct state {
    uint64_t r8;
    uint64_t r9;
    uint64_t r10;
    uint64_t r11;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;
    uint64_t rbp;
    uint64_t rsi;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;
    uint64_t vector;
    uint64_t err_code;
    uint64_t rip;
};

struct e20_entry {
    uint64_t addr;
    uint64_t len;
    uint32_t type;
    uint32_t res0;
}__attribute__((packed));
uint32_t apic_read(uint32_t reg);
int apic_write(uint32_t reg, uint32_t v);
bool is_bsp();
void invlpg(void* m);
uint64_t rdmsr(uint32_t msr);
void wrmsr(uint32_t msr, uint64_t value);
void halt();
void disable_interrupt();
void enable_interrupt();
size_t get_apic_id();
void recursion();
}
#endif
