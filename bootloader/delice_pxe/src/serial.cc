#include "serial.h"
#include "atomic.h"

//Initialize ports 
void serial::ComDevices::init(){
    for(auto &dev: device){
        auto com_id = (&dev - &device[0]);
        uint16_t port =
            *((reinterpret_cast<uint16_t*>(0x0400)) + com_id);
        if(port == 0){
            dev = 0;
            continue;
        }
        io::outb(port + 1, 0x00);
        io::outb(port + 3, 0x80);
        io::outb(port + 0, 0x01);
        io::outb(port + 1, 0x00);
        io::outb(port + 2, 0xC7);
        io::outb(port + 3, 0x03);
        io::outb(port + 4, 0x0B);
        dev = port;
    }
}

//Broadcast bytes to available serial ports.
void serial::ComDevices::write(const char byte){
    for (auto &dev: device){
        if(byte == '\n'){
            io::outb(dev, 0x0d);
        }
        if(dev == 0) continue;
        while((io::inb(dev + 5) & 0x20) == 0);
        io::outb(dev, byte);
    }
}
serial::ComDevices comdevices{};
extern "C" void _putchar_serial(char byte) {
    comdevices.write(byte);
}
/*
//Create shared device list that is shared among other threads
static Lock<serial::ComDevices> 
                spin(serial::ComDevices{});

//_putchar function is used within printf function to write bytes
//to serial interface. We can also change it to VGA interface.
extern "C" void _putchar_serial(char byte){
    spin.lock()->write(byte);
}
*/
void serial::Serial::init(){
    //initialize ports
    comdevices.init();
}
