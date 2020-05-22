#ifndef _CPU_H
#define _CPU_H
#include <cstddef>
#include <cstdint>
#define e20_addr 0x14000
namespace cpu {

struct registers {
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
    uint64_t rdi;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;
};

struct interrupt_frame {
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
};

struct e20_entry {
    uint64_t addr;
    uint64_t len;
    uint32_t type;
    uint32_t res0;
}__attribute__((packed));
static inline __attribute__((always_inline)) uint32_t apic_read(uint32_t reg)
{
    uint32_t *apic_base = (uint32_t*) 0xfee00000; // APIC base address
    return *((volatile uint32_t *)(apic_base + (reg / 4)));
}
static inline __attribute__((always_inline)) int apic_write(uint32_t reg, uint32_t v)
{
    volatile uint32_t *addr;
    uint32_t *apic_base = (uint32_t*) 0xfee00000; // APIC base address
    addr = (volatile uint32_t *)(apic_base + (reg / 4));
    asm volatile ("movl %1, %0"
        :"=m"(*addr):"r"(v):);

    return 0;
}
static inline __attribute__((always_inline)) size_t get_apic_id(){
    return (size_t)((apic_read(0x20) >> 24) & 0xff);
}

static inline __attribute__((always_inline)) void eoi(){
   cpu::apic_write(0xb0, 0x0); 
}


static inline __attribute__((always_inline)) void invlpg(void* m)
{
    asm volatile ( "invlpg (%0)" : : "b"(m) : "memory" );
}

static inline __attribute__((always_inline)) uint64_t rdmsr(uint32_t msr){
    uint32_t high, low;
    __asm__ ("rdmsr"
            : "=d"(high), "=a"(low)
            : "c"(msr)
            );
    return ((uint64_t)high << 32) | low;
}

static inline __attribute__((always_inline)) void wrmsr(uint32_t msr, uint64_t value)
{
    uint32_t low = value & 0xFFFFFFFF;
    uint32_t high = value >> 32;
    asm volatile (
        "wrmsr"
        :
        : "c"(msr), "a"(low), "d"(high)
    );
}
static inline __attribute__((always_inline)) void halt(){
    __asm__ ("hlt");
}

static inline __attribute__((always_inline)) bool is_bsp(){
    return (rdmsr(0x1b)  & (1 << 8)) != 0;
}

static inline __attribute__((always_inline)) void disable_interrupt(){
	__asm__ __volatile__ ("cli":::"memory", "cc");	
}
static inline __attribute__((always_inline)) void enable_interrupt(){
	__asm__ __volatile__ ("sti":::"memory", "cc");	
}

static inline __attribute__((always_inline)) void lfence(){
    asm volatile("lfence\n\t"
                 :
                 :
                 : "memory");
}

static inline __attribute__((always_inline)) void mfence(){
    asm volatile("mfence\n\t"
                 :
                 :
                 : "memory");
}

void recursion(size_t incr);
}
#endif
