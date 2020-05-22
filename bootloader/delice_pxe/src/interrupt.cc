#include <cstdint>
#include <cstddef>
#include <printf.h>
#include <misc/cpu.h>
#define INTERRUPT_GATE  0xe
#define TRAP_GATE       0xf
#define PAGE_FAULT      0xe
#define GP_FAULT        0xd
#define PMI_VEC         0x2
extern "C" uint64_t interrupt_handler(size_t vector, 
		struct cpu::interrupt_frame *frame, 
		size_t error_code, 
		struct cpu::registers *state);

extern uint64_t (*ex_handlers[32])();
extern "C" void default_trap_handler();

struct IDT_Desc {
    uint16_t offset_1;
    uint16_t selector;
    uint16_t ist:3;
    uint16_t res0:5;
    uint16_t type:4;
    uint16_t res1:1;
    uint16_t dpl:2;
    uint16_t present:1;
    uint16_t offset_2;
    uint32_t offset_3;
    uint32_t res2;
} __attribute__((packed));

struct idt_reg {
    uint16_t idt_limit;
    uint64_t idt_base_address;
} __attribute__((packed));

void memset(void *dst, char byte, size_t size){
	char *ptr = (char*)dst;
	for(size_t i = 0; i < size; i++){
		ptr[i] = byte;
	}
}
__attribute__((aligned(16)))
static struct IDT_Desc interrupt_desc_table[255];
__attribute__((aligned(64)))
static struct idt_reg idt;

void set_interrupt_gate(uint8_t vector, uint8_t type, uint64_t handler){
    struct IDT_Desc * idesc =
        (struct IDT_Desc *)&interrupt_desc_table[vector];
    memset(idesc, 0x0, sizeof(struct IDT_Desc));
    idesc->selector = 0x08;
    idesc->ist = 0x0;
    idesc->type = type;
    idesc->dpl = 0x0;
    idesc->present = 1;
    idesc->offset_1 = handler;
    idesc->offset_2 = handler >> 16;
    idesc->offset_3 = handler >> 32;
}

uint64_t interrupt_handler(size_t vector, 
        struct cpu::interrupt_frame *frame, 
        size_t error_code, 
        struct cpu::registers *state){
    printf("Hallo from interrupt! vector %lx %d\n", frame->rip, vector);
    switch(vector){
        case(PAGE_FAULT):
            printf("Page fault occurred!\n");
            while(1){}
            break;
        case(GP_FAULT):
            printf("General Protection fault occurred\n");
            while(1){};
            break;
        case(PMI_VEC):
            printf("PMI Interrupt occurred\n");
            cpu::eoi();
            return reinterpret_cast<uint64_t>(state);
            break;

    }
    cpu::eoi();
    return reinterpret_cast<uint64_t>(state);
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
