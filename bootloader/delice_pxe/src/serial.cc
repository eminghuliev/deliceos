#include "serial.h"
#include "atomic.h"

//Initialize ports 
void x86::serial::ComDevices::init(){
    for(auto &dev: device){
        auto com_id = (&dev - &device[0]);
        uint16_t port = *((reinterpret_cast<uint16_t*>(0x0400)) + com_id);
        if(port == 0){
            dev = 0;
            continue;
        }
        x86::io::outb(port + 1, 0x00);
        x86::io::outb(port + 3, 0x80);
        x86::io::outb(port + 0, 0x01);
        x86::io::outb(port + 1, 0x00);
        x86::io::outb(port + 2, 0xC7);
        x86::io::outb(port + 3, 0x03);
        x86::io::outb(port + 4, 0x0B);
        dev = port;
    }
}

//Broadcast bytes to available serial ports.
void x86::serial::ComDevices::write(const char byte){
    for (auto &dev: device){
        if(dev == 0) continue;
        while((x86::io::inb(dev + 5) & 0x20) == 0);
        x86::io::outb(dev, byte);
    }
}

//Create shared device list that is shared among other threads
static Lock<x86::serial::ComDevices> 
                spin(x86::serial::ComDevices{});

//_putchar function is used within printf function to write bytes
//to serial interface. We can also change it to VGA interface.
extern "C" void _putchar(char byte){
    spin.lock()->write(byte);
}

void x86::serial::Serial::init(){
    //initialize ports
    spin.lock()->init();
}
