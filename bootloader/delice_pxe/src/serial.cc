#include "serial.h"
void x86::serial::Serial::init(){
    x86::io::outb(PORT + 1, 0x00);
    x86::io::outb(PORT + 3, 0x80);
    x86::io::outb(PORT + 0, 0x01);
    x86::io::outb(PORT + 1, 0x00);
    x86::io::outb(PORT + 2, 0xC7);
    x86::io::outb(PORT + 3, 0x03);
    x86::io::outb(PORT + 4, 0x0B);
}

uint8_t x86::serial::Serial::is_transmit_empty() {
   return x86::io::inb(PORT + 5) & 0x20;
}
