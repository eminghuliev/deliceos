// Paging
#include <cstdint>
#include <printf.h>
#include <paging.h>
// Map 2MB

#define PAGE_SHIFT_2M 21

void map_addr(uint64_t vaddr, uint64_t phys){
     uint64_t indicies[] = {
        (vaddr >> 39) & 0x1ff,
        (vaddr >> 30) & 0x1ff,
        (vaddr >> 21) & 0x1ff,
        (vaddr >> 12) & 0x1ff,
     };
   
   struct pml4_entry *pml4e_addr = 
         (struct pml4_entry*)(FIRST_PML4_BASE + (uint8_t*)(indicies[0]));

    pml4e_addr->present = 1;
    pml4e_addr->writable = 1;
    pml4e_addr->pdpt_base = FIRST_PDPT_BASE + 0x1000;
    
    struct pdpt_entry *pdpte_addr =
        (struct pdpt_entry*) (pml4e_addr->pdpt_base + (uint8_t*)(indicies[1]));
    pdpte_addr->present = 1;
    pdpte_addr->writable = 1;
    pdpte_addr->pd_base = FIRST_PD_BASE + 0x1000; 


    struct pd2mb_entry *pd2mb_addr = 
        (struct pd2mb_entry*)(pdpte_addr->pd_base + (uint8_t*)(indicies[2]));
    pd2mb_addr->present = 1;
    pd2mb_addr->writable = 1;
    pd2mb_addr->page2m = phys >> PAGE_SHIFT_2M;

}
