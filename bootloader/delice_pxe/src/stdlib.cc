#include <stdlib.h>
void memset(void *dst, char byte, size_t size){
	char *ptr = (char*)dst;
	for(size_t i = 0; i < size; i++){
		ptr[i] = byte;
	}
}
