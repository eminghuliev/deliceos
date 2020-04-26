#include <types.h>
#include <misc/io.h>
#define PORT 0x3f8

namespace x86::serial {
class Serial {
    public:
        void init();
        void write();
        uint8_t is_transmit_empty();
};
}
