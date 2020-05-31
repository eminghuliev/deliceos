#ifndef _APIC_H
#define _APIC_H
#include <cstdint>
struct Apic {
    bool check_apic();
    void init();
    bool launch_ap(size_t core_id);
    size_t check_apic(size_t core_id);
    void enable_lapic();
    void lapic_init();
    void eoi();
};
#endif
