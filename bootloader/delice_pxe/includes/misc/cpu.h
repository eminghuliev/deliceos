#ifndef _CPU_H
#define _CPU_H
#include <cstddef>
#include <cstdint>
#include <tuple>
#define e20_addr 0x14000
#define intel() __asm__(".intel_syntax noprefix");
#define att() __asm__(".att_syntax");
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

[[gnu::always_inline]]
static inline uint32_t apic_read(uint32_t reg)
{
    uint32_t *apic_base = (uint32_t*) 0x13370000000; // APIC base address
    return *((volatile uint32_t *)(apic_base + (reg / 4)));
}
[[gnu::always_inline]]
static inline int apic_write(uint32_t reg, uint32_t v)
{
    volatile uint32_t *addr;
    uint32_t *apic_base = (uint32_t*) 0x13370000000; // APIC base address
    addr = (volatile uint32_t *)(apic_base + (reg / 4));
    asm volatile ("movl %1, %0"
        :"=m"(*addr):"r"(v):);

    return 0;
}
[[gnu::always_inline]]
static inline size_t get_apic_id(){
    return (size_t)((apic_read(0x20) >> 24) & 0xff);
}
[[gnu::always_inline]]
static inline void eoi(){
   cpu::apic_write(0xb0, 0x0); 
}

[[gnu::always_inline]]
static inline void invlpg(void* m)
{
    asm volatile ( "invlpg (%0)" : : "b"(m) : "memory" );
}
[[gnu::always_inline]]
static inline std::tuple<uint32_t, uint32_t, uint32_t, uint32_t> cpuid(uint32_t eax_ , uint32_t ecx_){
    uint32_t eax, ebx, ecx, edx;
    __asm__ volatile("cpuid"
    : "=a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx)
    : "a" (eax_), "c" (ecx_));
    return {eax, ebx, edx, ecx};
}
[[gnu::always_inline]]
static inline uint64_t rdmsr(uint32_t msr){
    uint32_t high, low;
    __asm__ ("rdmsr"
            : "=d"(high), "=a"(low)
            : "c"(msr)
            );
    return ((uint64_t)high << 32) | low;
}
[[gnu::always_inline]]
static inline void wrmsr(uint32_t msr, uint64_t value)
{
    uint32_t low = value & 0xFFFFFFFF;
    uint32_t high = value >> 32;
    asm volatile (
        "wrmsr"
        :
        : "c"(msr), "a"(low), "d"(high)
    );
}

static __inline__ int64_t rdtsc_s(void)
{
    unsigned a, d; 
    asm volatile("cpuid" ::: "%rax", "%rbx", "%rcx", "%rdx");
    asm volatile("rdtsc" : "=a" (a), "=d" (d)); 
    return ((unsigned long)a) | (((unsigned long)d) << 32); 
}

static __inline__ int64_t rdtsc_e(void)
{
    unsigned a, d; 
    asm volatile("rdtscp" : "=a" (a), "=d" (d)); 
    asm volatile("cpuid" ::: "%rax", "%rbx", "%rcx", "%rdx");
    return ((unsigned long)a) | (((unsigned long)d) << 32); 
}

[[gnu::always_inline]]
static inline void halt(){
    __asm__ ("hlt");
}

[[gnu::always_inline]]
static inline bool is_bsp(){
    return (rdmsr(0x1b)  & (1 << 8)) != 0;
}

[[gnu::always_inline]]
static inline void disable_interrupt(){
	__asm__ __volatile__ ("cli":::"memory", "cc");	
}

[[gnu::always_inline]]
static inline void enable_interrupt(){
	__asm__ __volatile__ ("sti":::"memory", "cc");	
}
[[gnu::always_inline]]
static inline void lfence(){
    asm volatile("lfence\n\t"
                 :
                 :
                 : "memory");
}
[[gnu::always_inline]]
static inline void mfence(){
    asm volatile("mfence\n\t"
                 :
                 :
                 : "memory");
}
[[gnu::always_inline]]
static inline uint64_t rdtsc(void)
{
    uint32_t high, low;
    __asm__ __volatile__ ("rdtsc" : "=a"(low), "=d"(high));
    return ((uint64_t)high << 32) | low;
}

void recursion(size_t incr);
}
#endif
