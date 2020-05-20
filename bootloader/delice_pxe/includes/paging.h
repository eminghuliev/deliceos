#define FIRST_PML4_BASE 0x32000
#define FIRST_PDPT_BASE 0x36000
#define FIRST_PD_BASE   0x37000
#define PAGE_PRESENT    (1 << 0)
#define PAGE_WRITE      (1 << 1)
#define PAGE_SIZE       (1 << 7)

struct pml4_entry {
    uint64_t present:1;
    uint64_t writable:1;
    uint64_t user_accessable:1;
    uint64_t page_writethrough:1;
    uint64_t page_cache:1;
    uint64_t res0:7;
    uint64_t pdpt_base:52;
}__attribute__((packed));

struct pdpt_entry {
    uint64_t present:1;
    uint64_t writable:1;
    uint64_t user_accessable:1;
    uint64_t page_writethrough:1;
    uint64_t page_cache:1;
    uint64_t res0:7;
    uint64_t pd_base:52;
}__attribute__((packed));

struct pd2mb_entry {
    uint64_t present:1;
    uint64_t writable:1;
    uint64_t user_accessable:1;
    uint64_t page_writethrough:1;
    uint64_t page_cache:1;
    uint64_t res0:2;
    uint64_t page_size:1;
    uint64_t res1:13;
    uint64_t page2m:43;
}__attribute__((packed));

void map_addr(uint64_t vaddr, uint64_t phys);
