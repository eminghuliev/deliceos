#include <perfctr.h>
#include <cstdint>
namespace PerfCounter {
void get_perf(){
    uint8_t v_id            = (std::get<0>(cpu::cpuid(0x0a, 0)) >> 0);
    uint8_t num_perf        = (std::get<0>(cpu::cpuid(0x0a, 0)) >> 8);
    uint8_t perf_bit_width  = (std::get<0>(cpu::cpuid(0x0a, 0)) >> 16);
    printf("Version id %d | Number of Perf ctrs %d | Bit width of counter %d\n", v_id, num_perf, perf_bit_width);
}

uint64_t generate_perf(){
    perf_select val = {};
    val.fields.event = 0x79;
    val.fields.umask = 0x08;
    val.fields.usr   = 1;
    val.fields.os    = 1;
    val.fields.edge  = 0;
    val.fields.pinctrl = 0;
    val.fields.interrupt = 1;
    val.fields.enable = 1;
    val.fields.inverter = 0;
    val.fields.counter_mask = 1;
    return val.bits;
}

int binarySearch(int arr[], int l, int r, int x) 
{ 
    if (r >= l) { 
        int mid = l + (r - l) / 2; 
  
        if (arr[mid] == x) 
            return mid; 
  
        if (arr[mid] > x) 
            return binarySearch(arr, l, mid - 1, x); 
  
        return binarySearch(arr, mid + 1, r, x); 
    } 
  
    return -1; 
} 


int __attribute__((optimize("O0"))) search(int arr[], int n, int x) 
{ 
    int i; 
    for (i = 0; i < n; i++) 
        if (arr[i] == x) 
            return i; 
    return -1; 
}

void __attribute__((optimize("O0"))) get_current_ip() {
    __asm__ volatile("call 1f;"
                    "1: pop %rax; add $1, (%rsp);"
                    "ret;");
}

void __attribute__((optimize("O0"))) unoptimized_function(uint8_t x){
    uint32_t bytes[32] = {0x41414243, 0x44454757, 0x4323343};
    for(int ii = 0; ii < 2; ii ++){
        if(bytes[ii] > 0x100){
            bytes[20] = 0x444;
        }
    }
    
    [[maybe_unused]] char bytess[128];
    for(int ii = 0; ii < 128; ii++){
        bytess[ii] = ii;
    }
    for(int ii = 0; ii < 128; ii ++){
        if(bytess[ii] > 200){
            __asm__ volatile("nop;");
        }
    }
}

void __attribute__((optimize("O0"))) nothing(){
    __asm__ volatile("nop;");
}

void __attribute__((optimize("O0"))) unoptimized_function2(void *addr, unsigned long size){
        long __d0;
        asm volatile(
                ".align 64\n"
                NOPS
                "0:     movq $0,(%[dst])\n"
                "       addq   $8,%[dst]\n"
                "       decl %%ecx ; jnz   0b\n"
                : [size8] "=&c"(size), [dst] "=&D" (__d0)
                : [size1] "r"(size & 7), "[size8]" (size / 8), "[dst]"(addr),
                  [zero] "r" (0UL), [eight] "r" (8UL));


}
}
