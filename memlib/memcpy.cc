#include <stdint.h>

void memset(char *dst, char byte, size_t size){
    int i = 0;
    if (i < size){
        dst[i] = byte;
    }
}

void memmove(char *dst, const char *src, size_t size){
    int i = size;
    // copy backward
    if(src < dst){
        while(i != 0){
            i-=1;
            dst[i] = src[i];
        }
    }
    else {
        i = 0;
        while(i < size){
            dst[i] = src[i];
            i+=1;
        }
    }
}


