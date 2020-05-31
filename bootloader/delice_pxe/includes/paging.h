#define FIRST_PML4_BASE 0x32000
#define FIRST_PDPT_BASE 0x36000
#define FIRST_PD_BASE   0x37000
#define PAGE_PRESENT    (1 << 0)
#define PAGE_WRITE      (1 << 1)
#define PAGE_SIZE_2MB       (1 << 7)
#define PAGE_UC         (1 << 4)
void map_addr(uint64_t vaddr, uint64_t phys, uint64_t flags);
