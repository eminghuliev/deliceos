#include <cstdint>
#include <tuple>
#include <misc/cpu.h>
#include <printf.h>
namespace PerfCounter {

typedef union {
    struct {
        uint8_t event           :8;
        uint8_t umask           :8;
        uint8_t usr             :1;
        uint8_t os              :1;
        uint8_t edge            :1;
        uint8_t pinctrl         :1;
        uint8_t interrupt       :1;
        uint8_t _               :1;
        uint8_t enable          :1;
        uint8_t inverter        :1;
        uint8_t counter_mask    :8;
    } fields;
    uint64_t bits;
} perf_select;

// Definitions 
void get_perf();
void unoptimized_function(int x);
void unoptimized_function2();
uint64_t generate_perf();

// PMC definitions
static inline __attribute__((always_inline)) uint64_t read(uint32_t id){
    uint32_t low, high;
    asm volatile ("rdpmc": "=d"(high), "=a"(low): "c"(id) : "memory");
    cpu::mfence();
    cpu::lfence();
    return ((uint64_t)high << 32) | (uint64_t)low;
}
// Configure APIC to deliver NMI for PMC
static inline __attribute__((always_inline)) void enable_pmi_nmi(){
    cpu::apic_write(0x340, 0b100 << 8);
}

static inline __attribute__((always_inline)) void set_event_counts(uint64_t val){ 
    cpu::wrmsr(0x309, val);
}
static inline __attribute__((always_inline)) void set_fixed_ctrl(){
    cpu::wrmsr(0x38d, 0xb0);
}

static inline __attribute__((always_inline)) void set_perf_globalctrl(){
    cpu::wrmsr(0x38f, 0x000000001);
    // Enable to Fix #1
    //cpu::wrmsr(0x38f, 0x200000001);
}

static inline __attribute__((always_inline)) void disable_perf_globalctrl(){
    cpu::wrmsr(0x38f, 0);
}

static inline __attribute__((always_inline)) void set_perfselect(uint64_t val){
    cpu::wrmsr(0x186, val);
}

static inline __attribute__((always_inline)) void freeze_on_overflow(){
    cpu::wrmsr(0x1d9, cpu::rdmsr(0x1d9) | (1 << 12));
}
static inline __attribute__((always_inline)) void disable_noclear(){
    cpu::wrmsr(0x186, 0x0);
}

static inline __attribute__((always_inline)) void clear(){
    cpu::wrmsr(0x186, 0x0);
	cpu::wrmsr(0xc1, 0x0);
}

static inline __attribute__((always_inline)) void reset(){
    cpu::wrmsr(0x390, cpu::rdmsr(0x38e));
}
}
