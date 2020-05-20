#include "misc/cpu.h"

namespace cpu {
void invlpg(void* m)
{
    asm volatile ( "invlpg (%0)" : : "b"(m) : "memory" );
}
uint64_t rdmsr(uint32_t msr){
    uint32_t high, low;
    __asm__ ("rdmsr"
            : "=d"(high), "=a"(low)
            : "c"(msr)
            );
    return ((uint64_t)high << 32) | low;
}

void recursion(){
    recursion();
}

void enable_interrupt(){
    __asm__ __volatile__ ("sti":::"memory", "cc");
}

void disable_interrupt(){
    __asm__ __volatile__ ("cli":::"memory", "cc");
}

void wrmsr(uint32_t msr, uint64_t value)
{
    uint32_t low = value & 0xFFFFFFFF;
    uint32_t high = value >> 32;
    asm volatile (
        "wrmsr"
        :
        : "c"(msr), "a"(low), "d"(high)
    );
}

int apic_write(uint32_t reg, uint32_t v)
{
    volatile uint32_t *addr;
    uint32_t *apic_base = (uint32_t*) 0xfee00000; // APIC base address
    addr = (volatile uint32_t *)(apic_base + (reg / 4));
    asm volatile ("movl %1, %0"
        :"=m"(*addr):"r"(v):);

    return 0;
}

uint32_t apic_read(uint32_t reg)
{
    uint32_t *apic_base = (uint32_t*) 0xfee00000; // APIC base address
    return *((volatile uint32_t *)(apic_base + (reg / 4)));
}
void halt(){
    __asm__ ("hlt");
}

size_t get_apic_id(){
    return (size_t)((cpu::apic_read(0x20) >> 24) & 0xff);
}

bool is_bsp(){
    return (rdmsr(0x1b)  & (1 << 8)) != 0;
}
}
