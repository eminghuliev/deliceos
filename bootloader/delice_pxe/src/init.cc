#include <serial.h>
#include <misc/cpu.h>
#include <misc/cpuid.h>
#include <apic.h>
#include <printf.h>
#include <misc/init.h>
#include <atomic>
#include <paging.h>
#include <interrupt.h>
extern "C" void start_kernel(){
    // Initialize all entries in init_array section
    static_init();
    cpu::disable_interrupt();
    
    //uint32_t *addr = (uint32_t*)((uint8_t*)0xb8000 + (cx * 2));
    //*addr = 0x0f41;

    static std::atomic_uint CORE_ID {0};
    // Set-up serial interface
    serial::Serial serial;
    serial.init();
    auto core_id = CORE_ID.fetch_add(1, std::memory_order_seq_cst);
    uint32_t esp;
    __asm__ ("mov %%esp, %0;" : "=r" (esp));
    if(cpu::is_bsp()){
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
        //map_addr(0xfee00020, 0xfee00020);
        printf("It's BSP processor\n");
        Apic apic;
        if(apic.check_apic()){
            printf("It supports APIC!\n");
        }
        load_idt();
        cpu::enable_interrupt();

        apic.init();
        apic.enable_lapic();
        //apic.lapic_init();
	//cpu::recursion();
        //printf("Hi guyz bross\n");
        //apic.launch_ap(core_id + 1);
        //__asm__("int $250");
        uint64_t *addrsix = (uint64_t*)0x1337debeef;
        *addrsix = 0xbeef;
    	printf("After interrupt bro\n");
    }

    if(cpu::get_apic_id() == 1){
        printf("Hello from core 1 core_id %d\n", core_id);
    }

    if(cpu::get_apic_id() == 2){
        printf("Hello from core 2 core_id %d\n", core_id);
    }
    if(cpu::get_apic_id() == 3){
        printf("Hello from core 3 core_id %d\n", core_id);
    }
    // We will release stack in the end of each core procedure
    uint8_t *stack_wait = (uint8_t*)0x7e00;
    *stack_wait = 0x1;
    // CPU core enters into hlt
    __asm__("cli; hlt");
}
