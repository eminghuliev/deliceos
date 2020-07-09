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

#define BUF_SIZE (64*1024)
static char buf[BUF_SIZE];

extern "C" void __attribute__((optimize("O0"))) start_kernel(){
    // Initialize all entries in init_array section
    static_init();
    cpu::disable_interrupt();
    static std::atomic_uint CORE_ID {0};
    // Set-up serial interface
    serial::Serial serial;
    serial.init();
    [[maybe_unused]] auto core_id = CORE_ID.fetch_add(1, std::memory_order_seq_cst);
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
//         intel();
//         // disable cache
//         __asm__ volatile ("mov eax, cr0;"
//                           "or eax, (0x1 << 30);"
//                           "mov cr0, eax;");
//         att();

        ::PerfCounter::get_perf();
        ::PerfCounter::enable_pmi_nmi();
        for(int ii = 0; ii < 1; ii++){
            ::PerfCounter::disable_perf_globalctrl();
            ::PerfCounter::reset();
            ::PerfCounter::clear();
            __asm__ volatile("wbinvd");
            ::PerfCounter::set_perfselect(::PerfCounter::generate_perf());
            //::PerfCounter::set_event_counts((1ULL << 48) - 2);
            ::PerfCounter::set_perf_globalctrl();
            //::PerfCounter::set_fixed_ctrl();
            /*intel();
            __asm__ volatile("lea rsi, [rsb + 2];call rsb;"
                                "rsb: push rsi; ret;");
            att();*/
            // Enable Indirect Branch Predictor Barrier
            //cpu::wrmsr(0x48, 0x02);
            //cpu::wrmsr(0x49, 0x01);
            //__asm__ volatile ("clflush (%0)" :: "r"(&(buff[25])));
            uint64_t val = cpu::rdmsr(0x1d9);
            ::PerfCounter::unoptimized_function2(buf, BUF_SIZE);
            ::PerfCounter::disable_perf_globalctrl();
            printf("VAL %d\n", val);
            printf("PMC counter val %d\n", PerfCounter::read(0));
        }
        // Send IPI to All cores
        //apic.launch_ap(core_id + 1);
        //printf("CPU %d\n", cpu::get_apic_id());
    }
    else {
        printf("Dragon rocket CPU Core id: %d\n", cpu::get_apic_id());
    }
    // We will release stack in the end of each core procedure
    uint8_t *stack_wait = (uint8_t*)0x7e00;
    *stack_wait = 0x1;
    // CPU core enters into hlt
    __asm__("cli; hlt");
}
