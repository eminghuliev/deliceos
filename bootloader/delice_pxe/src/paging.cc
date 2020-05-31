// Paging
#include <cstdint>
#include <stdlib.h>
#include <paging.h>
#include <misc/cpu.h>
#include <printf.h>
#define APIC_PDPTE 0x22000
#define APIC_PDE 0x23000

void map_addr(uint64_t vaddr, uint64_t phys, uint64_t flags){
    uint64_t indicies[] = {
        (vaddr >> 39) & 0x1ff,
        (vaddr >> 30) & 0x1ff,
        (vaddr >> 21) & 0x1ff,
        (vaddr >> 12) & 0x1ff,
    };
    uint64_t *pml4_addr = ((uint64_t*)FIRST_PML4_BASE) + indicies[0];
    memset(pml4_addr, 0x0, sizeof(uint64_t));
    *pml4_addr = PAGE_PRESENT | PAGE_WRITE | APIC_PDPTE;

    uint64_t *pdpte_addr = ((uint64_t*)APIC_PDPTE) + indicies[1]; 
    memset(pdpte_addr, 0x0, sizeof(uint64_t));
    *pdpte_addr = PAGE_PRESENT | PAGE_WRITE | APIC_PDE;


    uint64_t *pd_entry = ((uint64_t*)APIC_PDE) + indicies[2];
    memset(pd_entry, 0x0, sizeof(uint64_t));
    *pd_entry = flags | PAGE_SIZE_2MB | phys;
}
