#include <cstdint>
#include <cstddef>
#include <printf.h>
#include <misc/cpu.h>
#define INTERRUPT_GATE              0xe
#define TRAP_GATE                   0xf

extern "C" void interrupt_handler(struct cpu::state *state);

extern uint64_t (*ex_handlers[32])();
extern "C" void default_trap_handler();

struct IDT_Desc {
    uint16_t offset_0_15;
    uint16_t selector;
    uint16_t ist:3;
    uint16_t reserved0:5;
    uint16_t type:4;
    uint16_t reserved1:1;
    uint16_t dpl:2;
    uint16_t present:1;
    uint16_t offset_16_31;
    uint32_t offset_32_63;
    uint32_t reserved2;
}__attribute__((packed));

struct idt_info {
    uint16_t idt_limit;
    uint64_t idt_base_address;
}__attribute__((packed));

void
memset(void * dst, uint8_t val, uint64_t size)
{
    uint64_t idx = 0;
    uint8_t * ptr = (uint8_t *)dst;
    for (; idx < size; idx++) {
        ptr[idx] = val;
    }
}

__attribute__((aligned(16)))
static struct IDT_Desc interrupt_desc_table[255];
__attribute__((aligned(64)))
static struct idt_info idt;

void set_interrupt_gate(uint8_t vector, uint8_t type, uint64_t callback){
    struct IDT_Desc * idesc =
        (struct IDT_Desc *)&interrupt_desc_table[vector];
    memset(idesc, 0x0, sizeof(struct IDT_Desc));
    idesc->selector = 0x08;
    idesc->ist = 0x0;
    idesc->type = type;
    idesc->dpl = 0x0;
    idesc->present = 1;
    idesc->offset_0_15 = callback;
    idesc->offset_16_31 = callback >> 16;
    idesc->offset_32_63 = callback >> 32;
}

void eoi(){
   cpu::apic_write(0xb0, 0x0); 
}

void interrupt_handler(struct cpu::state *state){
    printf("Hallo from interrupt! vector %lx %d\n",state->rip, state->vector);
    eoi();
    cpu::halt();
}

void load_idt(void)
{
    for(uint8_t ii = 0; ii < 32; ii++){
        set_interrupt_gate(ii, INTERRUPT_GATE, (uint64_t)ex_handlers[ii]);
    }
    for(uint16_t ii = 32; ii < 256; ii++){
        set_interrupt_gate(ii, TRAP_GATE, (uint64_t)default_trap_handler);
    }
    idt.idt_limit = sizeof(interrupt_desc_table) - 1;
    idt.idt_base_address = (uint64_t)interrupt_desc_table;
    __asm__ volatile("lidt %0;"
                     :
                     :"m"(idt)
                     :"cc");
}
