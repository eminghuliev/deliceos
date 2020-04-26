#include <types.h>
namespace x86::io {
    inline void outb(int16_t port, int8_t val){
        asm volatile("outb %%al, %%dx;"
                    : : "a" (val),
                    "d" (port)
                );
    }
    inline uint8_t inb(uint16_t port){ 
        uint8_t ret;
        asm volatile("inb %%dx, %%al;"
                    :"=a" (ret)
                    :"d" (port));
        return ret;
    }
}
