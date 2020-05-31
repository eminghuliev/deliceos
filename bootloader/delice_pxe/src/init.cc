#include <serial.h>
#include <misc/cpu.h>
#include <misc/cpuid.h>
#include <apic.h>
#include <printf.h>
#include <misc/init.h>
#include <atomic>
#include <paging.h>
#include <interrupt.h>
#include <perfctr.h>
#include <screen.h>
extern "C" void start_kernel(){
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
        printf("Delice OS Bootloader\n");
        /*
        // Get count of e20 entries
        uint8_t entry_count = 
                (*(reinterpret_cast<uint8_t*>(e20_addr + 0x200))) - 1;
        // Get memory entries from e20 map
        for(uint8_t ii = 0; ii < entry_count; ii++){
            cpu::e20_entry* entry_addr = 
                reinterpret_cast<cpu::e20_entry*>(e20_addr) + ii;
            printf("e820 entry addr entry %lx %x %d\n", entry_addr->addr, 
                                        entry_addr->len, entry_addr->type);
        }*/
        printf("BSP processor ready\n"); 
        Apic apic;
        if(apic.check_apic()){
            //printf("APIC is supported");
        }
        load_idt();
        cpu::enable_interrupt();
        apic.init();
        apic.enable_lapic();
        ::PerfCounter::get_perf();
        ::PerfCounter::enable_pmi_nmi();
        /*[[maybe_unused]] void (*junk_ptr[2])() = {&PerfCounter::unoptimized_function, 
                                &PerfCounter::unoptimized_function2};
        */
        //int arr[] = {2, 160, 110, 180, 81, 170, 105, 89, 130, 120, 105, 300, 480, 490, 420};
        for(int ii = 0; ii < 30; ii++){
            ::PerfCounter::disable_perf_globalctrl();
            ::PerfCounter::reset();
            ::PerfCounter::clear();
            __asm__ volatile ("WBINVD");
            ::PerfCounter::set_perfselect(::PerfCounter::generate_perf());
            //::PerfCounter::set_event_counts((1ULL << 48) - 2);
            //::PerfCounter::set_fixed_ctrl();
            ::PerfCounter::set_perf_globalctrl();
            //junk_ptr[ii]();
            uint64_t start = cpu::rdtsc();
            intel();
            __asm__ volatile("lfence;"
                            "mov edx, 0x5;"
                            "mov eax, 0x400;"
                            "xor ecx, ecx;"
                            );
            att();
            //::PerfCounter::unoptimized_function(arr[ii]);
            uint64_t stop = cpu::rdtsc();
            //printf("PMC counter val %d\n", PerfCounter::read(0));
            printf("Cycles %d, %d\n",ii, stop - start);
        }
        /*::PerfCounter::enable_pmi_nmi();
        void (*junk_ptr[2])() = {&PerfCounter::unoptimized_function, 
                                &PerfCounter::unoptimized_function2};
        for(int ii = 0; ii < 2; ii++){
            ::PerfCounter::reset();
            ::PerfCounter::clear();
            ::PerfCounter::set_event_counts((1ULL << 48) - 2);
            ::PerfCounter::set_fixed_ctrl();
            ::PerfCounter::set_perfselect(::PerfCounter::generate_perf());
            ::PerfCounter::set_perf_globalctrl();
            //junk_ptr[ii]();
            __asm__ volatile("cmp %eax, %eax;"
                    "jnz 1f;"
                    "1: mov $0, %eax");
            ::PerfCounter::disable_perf_globalctrl();
            while(cpu::rdmsr(0x38e) == 0){}
            printf("What di fak? ass %x\n", cpu::rdmsr(0x30a));
            printf("PMC counter val %d\n", PerfCounter::read(0));
        } */
        // Send IPI to All cores
        //apic.launch_ap(core_id + 1);
    }

    if(cpu::get_apic_id() == 3){
        printf("Hello from core 3 core_id %d\n", core_id);
    }

    /*if(cpu::get_apic_id() == 2){
        printf("Hello from core 2 core_id %d\n", core_id);
    }
    if(cpu::get_apic_id() == 3){
        printf("Hello from core 3 core_id %d\n", core_id);
    }*/
    // We will release stack in the end of each core procedure
    //uint8_t *stack_wait = (uint8_t*)0x7e00;
    //*stack_wait = 0x1;
    // CPU core enters into hlt
    __asm__("cli; hlt");
}
