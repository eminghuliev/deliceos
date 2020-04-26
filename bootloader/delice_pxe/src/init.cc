#include "serial.h"
extern "C" void start_kernel(){
    char test[] = "GOUP";
    int i = 0;
    
    x86::serial::Serial serial;
    serial.init();
    while(test[i] != 0){
        while (serial.is_transmit_empty() == 0);
        x86::io::outb(PORT, static_cast<uint8_t>(test[i]));
        i++;
    }
}
