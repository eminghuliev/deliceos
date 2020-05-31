#include <cstddef>
#include <misc/cpu.h>
#include <optional>
#include <printf.h>
#include <apic.h>
#define MADT_LAPIC 0x00
#define MADT_IOAPIC 0x01
#define MAX_CPUS 18
#define APIC_LVT_MASKED 0x10000

uint8_t cpu_nodes[MAX_CPUS] = {};

struct rsdp
{
    char              signature[8];
    uint8_t           checksum;
    uint8_t           oem_id[6];
    uint8_t           revision;
    uint32_t          rsdt_addr;
    uint32_t          length;
    uint64_t          xsdt;
    uint8_t           checksum2;
    uint8_t           _[3];
} __attribute__((packed));

struct acpisdt {
    char            signature[4];
    uint32_t        length;
    uint8_t         revision;
    uint8_t         checksum;
    uint8_t         oem_id[6];
    uint8_t         oem_table_id[8];
    uint32_t        oem_revision;
    uint32_t        creator_id;
    uint32_t        creator_revision;
    uint32_t        payload[];
} __attribute__((packed));


struct acpi_madt
{
  uint32_t lapic_addr;
  uint32_t _;
  uint8_t payload[];
} __attribute__((packed));

int memcmp(char *src, char *dest, size_t size){
    while ((*src == *dest) && (size > 0))
    {
        ++src;
        ++dest;
        --size;
    }
    if(size == 0){
        return 0;
    }
    return *src - *dest;
}

bool Apic::check_apic(){
    [[maybe_unused]] auto [eax, ebx, edx, ecx] = cpu::cpuid(1, 0);
    return edx & ( 1 << 9);
}
std::optional<char*> check_rsdt(uint32_t low, uint32_t high){
    for(char *ii = reinterpret_cast<char*>(low); 
                ii < reinterpret_cast<char*>(high); ii+=16){
        if(memcmp(ii, (char*)"RSD PTR ", 8) == 0){
            return ii;
        }
    }
    return std::nullopt;
}

template<size_t I = 0, typename... Tp>
bool iterate_check(std::tuple<Tp...>& t){
    std::optional<char *> rdst_ptr = 
            check_rsdt(std::get<I>(t).first, std::get<I>(t).second);
    if(rdst_ptr){
        // Get RSDT PTR struct
        struct rsdp *rdsp_ptr = (struct rsdp*)(*rdst_ptr);
        // Get RSDT address
        uint32_t *rsdt_addr = reinterpret_cast<uint32_t*>(rdsp_ptr->rsdt_addr);
        // Get RSDT header
        struct acpisdt *rsdt_hdr = (struct acpisdt*)rsdt_addr;
        // Get count of ACPI Standart Header entries in RSDT header
        int entries = (rsdt_hdr->length - sizeof(struct acpisdt)) / 4;
        uint32_t *entries_ptr = (uint32_t*)rsdt_hdr->payload;
        for(auto ii = 0; ii < entries; ii++){
            // Get ACPI header
            struct acpisdt *acpi_ptr = 
                        reinterpret_cast<struct acpisdt*>(entries_ptr[ii]); 
            if(memcmp(acpi_ptr->signature, (char*)"APIC", 4) == 0){
                
                struct acpi_madt *madt_ptr = 
                                    (struct acpi_madt*)acpi_ptr->payload;
                //printf("Local APIC address %lx\n", madt_ptr->lapic_addr);
                uint8_t *end_apic = (((uint8_t*)acpi_ptr) + acpi_ptr->length );
                while((uint8_t*)madt_ptr < end_apic){
                    uint8_t type = madt_ptr->payload[0];
                    uint8_t len  = madt_ptr->payload[1];
                    switch(type){
                        case MADT_LAPIC:
                            uint8_t lapic_id = madt_ptr->payload[2];
                            cpu_nodes[lapic_id] = lapic_id;
                            break;
                    }
                    
                    madt_ptr = (struct acpi_madt*)((uint8_t*)madt_ptr + len);
                }
            }
        }
        return true;
    }
    if constexpr(I+1 != sizeof...(Tp))
        iterate_check<I+1>(t);
    return false;
}

size_t Apic::check_apic(size_t core_id) {
    size_t c_id = 0;
    for(int ii = 0; ii <= MAX_CPUS;){
        if(cpu_nodes[core_id] != core_id){
            continue;
        }
        if (c_id == core_id) {
            return ii;
        }
        c_id+=1;
    }
    return 0;
}

void Apic::eoi(){
    cpu::apic_write(0xb0, 0x0);
}

void mask_lvt(uint32_t reg_addr){
    uint32_t reg = cpu::apic_read(reg_addr);
    reg |= APIC_LVT_MASKED;
    cpu::apic_write(reg_addr, reg);
}

void Apic::enable_lapic(){

    // Set global APIC enable flag in APIC_BASE MSR
    cpu::wrmsr(0x1b, 0xfee00000 | (1 << 11));

    // Set APIC Software Enable/Disable in Spurious-Interrupt Vector Register
    cpu::apic_write(0xf0, (uint32_t)(1 << 8) | 0xff); 
    cpu::apic_write(0x80, 0x0);
    cpu::apic_write(0xb0, 0x0);

}

void Apic::lapic_init(){
    // Configure APIC to deliver NMIs
    // Primary configuration

    // New configuration
    //__asm__("wrmsr" : : "c"(0xc1),  "a"(0x0), "d"(0x0));
    //__asm__("wrmsr" : : "c"(0x4c1), "a"((1 << 48) - 10000), "d"(0x0));
    //cpu::wrmsr(0x4c1, (1ULL << 48));
    //__asm__("wrmsr" : : "c"(0x186), "a"(0x5301c2), "d"(0x0));
    //__asm__("wrmsr" : : "c"(0x38F), "a"(0x00000001), "d"(0x00000000));


    //__asm__("wrmsr" : : "c"(0x186), "a"(0x005100C0), "d"(0x0));
    //__asm__("wrmsr" : : "c"(0xc1),  "a"(0xFFFFD8F0), "d"(0x0));
    //__asm__("wrmsr" : : "c"(0x309), "a"(0x80000000), "d"(0xffff));
    //__asm__("xor %edx, %edx");
    //__asm__("wrmsr" : : "c"(0x309), "a"(0x80000000), "d"(0xffff));
    //__asm__("wrmsr" : : "c"(0x38d), "a"(0xa), "d"(0x0));
}

bool Apic::launch_ap(size_t core_id){
    //auto apic_id = check_apic(core_id);
    //cpu::apic_write(0x310, ((uint32_t)apic_id) << 24);
    cpu::apic_write(0x300, 0xc4500);
    //cpu::apic_write(0x310, ((uint32_t)apic_id) << 24);
    cpu::apic_write(0x300, 0xc4608);
    //cpu::apic_write(0x310, ((uint32_t)apic_id) << 24);
    cpu::apic_write(0x300, 0xc4608);
    return false;
}

void Apic::init(){
    uint32_t paddr;
    std::optional<char*> addr_rptr;
    paddr = (*((uint16_t*)(0x40e))) << 4;
    std::tuple t (
                std::pair(paddr,paddr + 1024), 
                std::pair(0xe0000, 0x100000)
            );
   //Firstly, search EBDA address space for RSDP and
   //we can also check from 0xe00000 to 0xfffff read-only address space
   // Pass EBDA address low and high range (from ebda to ebda + 1024)
  iterate_check(t); 
}
