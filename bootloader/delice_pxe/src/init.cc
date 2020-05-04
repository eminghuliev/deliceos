#include "serial.h"
#include "printf.h"
#include "misc/cpu.h"
#include "misc/cpuid.h"
#include "misc/init.h"
extern "C" void start_kernel(){
    //Initialize all entries in init_array section
    static_init();

    //Set-up serial interface
    x86::serial::Serial serial;
    serial.init();
    
    printf("Delice OS loading ...\n");

    //Get count of e20 entries
    uint8_t entry_count = 
            (*(reinterpret_cast<uint8_t*>(e20_addr + 0x200))) - 1;
    //Get memory entries from e20 map
    for(uint8_t ii = 0; ii < entry_count; ii++){
        x86::cpu::e20_entry* entry_addr = 
            reinterpret_cast<x86::cpu::e20_entry*>(e20_addr) + ii;
        printf("e820 entry addr %lx\n", entry_addr->addr);
    }

    //CPU enters into hlt
    __asm__("cli; hlt");
}
