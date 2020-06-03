#include <serial.h>
#include <misc/cpu.h>
#include <misc/cpuid.h>
#include <apic.h>
#include <printf.h>
#include <misc/init.h>
#include <atomic.h>
#include <paging.h>
#include <interrupt.h>
#include <perfctr.h>
#include <screen.h>

extern "C" void __attribute__((optimize("O0"))) start_kernel(){
    // Initialize all entries in init_array section
    static_init();
    cpu::disable_interrupt();
    static std::atomic_uint CORE_ID {0};
    // Set-up serial interface
    serial::Serial serial;
    serial.init();
    //clear_screen();
    auto core_id = CORE_ID.fetch_add(1, std::memory_order_seq_cst);
    uint32_t esp;
    __asm__ ("mov %%esp, %0;" : "=r" (esp));
    // Map APIC Base address with UC field
    map_addr(0x13370000000, 
            0xfee00000, 
            PAGE_PRESENT | PAGE_WRITE | PAGE_UC);

    if(cpu::is_bsp()){
        //printf("Delice OS Bootloader\n");
        // Get count of e20 entries
        uint8_t entry_count = 
                (*(reinterpret_cast<uint8_t*>(e20_addr + 0x200))) - 1;
        // Get memory entries from e20 map
        for(uint8_t ii = 0; ii < entry_count; ii++){
            cpu::e20_entry* entry_addr = 
                reinterpret_cast<cpu::e20_entry*>(e20_addr) + ii;
            printf("e820 entry addr entry %lx %x %d\n", entry_addr->addr, 
                                        entry_addr->len, entry_addr->type);
        }
        printf("BSP processor ready\n"); 
        Apic apic;
        if(apic.check_apic()){
            printf("APIC is supported\n");
        }
        load_idt();
        cpu::enable_interrupt();
        apic.init();
        apic.enable_lapic();
        // Send IPI to All cores
        apic.launch_ap(core_id + 1);
        //printf("CPU %d\n", cpu::get_apic_id());
    }
    else {
        printf("My Rocket CPU Core id: %d\n", cpu::get_apic_id());
    }
    // We will release stack in the end of each core procedure
    uint8_t *stack_wait = (uint8_t*)0x7e00;
    *stack_wait = 0x1;
    // CPU core enters into hlt
    __asm__("cli; hlt");
}
