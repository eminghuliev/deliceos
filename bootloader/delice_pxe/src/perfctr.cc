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
    val.fields.event = 0x02;
    val.fields.umask = 0x01;
    val.fields.usr   = 1;
    val.fields.os    = 1;
    val.fields.edge  = 0;
    val.fields.pinctrl = 0;
    val.fields.interrupt = 1;
    val.fields.enable = 1;
    val.fields.counter_mask = 0;
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

void __attribute__((optimize("O0"))) unoptimized_function(int x){
    intel();
    __asm__ volatile("add dword ptr[rsp + 0x8], 4");
    att();
}

void __attribute__((optimize("O0"))) nothing(){
    __asm__ volatile("nop;");
}

void __attribute__((optimize("O0"))) unoptimized_function2(){
    for(int ii = 0; ii < 255; ii++){
        nothing();
    }
}
}
