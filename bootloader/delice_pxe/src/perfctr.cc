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
    val.fields.event = 0xc4;
    val.fields.umask = 0x00;
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

void __attribute__((optimize("O0"))) unoptimized_function(int x){
    /*uint32_t *addr = (uint32_t*)0xb8000;
    for(int ii = 0; ii < 250; ii++){
        if(*(addr + ii)){
            __asm__ volatile("nop");
        }
    }
    __asm__ volatile("cmp %eax, %eax;"
                    "jnz 1f;"
                    "1: mov $0, %eax");
    */
    /*int arr[] = { 2, 3, 4, 10, 40, 50, 70, 81, 82, 84, 89, 90 , 100, 102, 103, 104, 105, 106, 110, 120, 130, 140, 150, 160, 170, 180, 200, 220, 230, 240, 260, 300, 320, 380, 390, 400, 420, 430, 440, 450, 480, 490, 500}; 
    int n = sizeof(arr) / sizeof(arr[0]); 
    //binarySearch(arr, 0, n - 1, x);
    search(arr, n, x);
    //printf("Res %d\n", result);
*/
}

void __attribute__((optimize("O0"))) unoptimized_function2(){
    uint32_t *addr = (uint32_t*)0xb8000;
    for(int ii = 0; ii < 255; ii++){
        if(*(addr + ii)){
            __asm__ volatile("nop");
        }
    }
}
}
