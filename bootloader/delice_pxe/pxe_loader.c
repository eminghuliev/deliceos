#define oop "hiiguyz"
int a = 0x41;
void exit();

void entry(){
    int *addr = (int*)0xb8000;
    *addr = 0x41f0;
    exit();
    for(;;){
        __asm__("hlt");
    }
}

