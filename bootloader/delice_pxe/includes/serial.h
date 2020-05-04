#ifndef _SERIAL_H
#define _SERIAL_H
#include <cstdint>
#include <misc/io.h>
#define PORT 0x3f8

namespace x86::serial {

struct ComDevices {
    uint16_t device[4];
    void write(const char byte);
    void init();
};
    
class Serial {
    public:
        void init();
};
}
#endif
