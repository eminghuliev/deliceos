global default_trap_handler
global ex_handlers
extern interrupt_handler

default_trap_handler:
    mov rax, 0xb8000
    mov dword[rax], 0x0f42
    jmp $

entry_vector:
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push rbp
    push r15
    push r14
    push r13
    push r12
    push r11
    push r10
    push r9
    push r8
    mov edi, dword[rsp + 120] ; vector code
    mov edx, dword[rsp + 128] ; error code
    mov rcx, rsp ; stack register
    lea rsi, [rsp + 136] ; interrupt frame
    call interrupt_handler
    mov rsp, rax
    pop r8
    pop r9
    pop r10
    pop r11
    pop r12
    pop r13
    pop r14
    pop r15
    pop rbp
    pop rsi
    pop rdi
    pop rdx
    pop rcx
    pop rbx
    pop rax
    add rsp, 16
    cli
    hlt
    ; It will be changed with iretq
    iret

%macro EXCEPTION_ENTRY_ERROR_CODE 1
align 8
exception%1_handler:
    cli
    push byte %1
    jmp entry_vector
%endmacro

%macro EXCEPTION_ENTRY 1
align 8
exception%1_handler:
    cli
    push byte 0
    push byte %1
    jmp entry_vector
%endmacro
EXCEPTION_ENTRY 0
EXCEPTION_ENTRY 1
EXCEPTION_ENTRY 2
EXCEPTION_ENTRY 3
EXCEPTION_ENTRY 4
EXCEPTION_ENTRY 5
EXCEPTION_ENTRY 6
EXCEPTION_ENTRY 7
EXCEPTION_ENTRY_ERROR_CODE 8
EXCEPTION_ENTRY 9
EXCEPTION_ENTRY_ERROR_CODE 10
EXCEPTION_ENTRY_ERROR_CODE 11
EXCEPTION_ENTRY_ERROR_CODE 12
EXCEPTION_ENTRY_ERROR_CODE 13
EXCEPTION_ENTRY_ERROR_CODE 14
EXCEPTION_ENTRY 15
EXCEPTION_ENTRY 16
EXCEPTION_ENTRY_ERROR_CODE 17
EXCEPTION_ENTRY 18
EXCEPTION_ENTRY 19
EXCEPTION_ENTRY 20
EXCEPTION_ENTRY 21
EXCEPTION_ENTRY 22
EXCEPTION_ENTRY 23
EXCEPTION_ENTRY 24
EXCEPTION_ENTRY 25
EXCEPTION_ENTRY 26
EXCEPTION_ENTRY 27
EXCEPTION_ENTRY 28
EXCEPTION_ENTRY 29
EXCEPTION_ENTRY 30
EXCEPTION_ENTRY 31

ex_handlers:
    dq exception0_handler
    dq exception1_handler
    dq exception2_handler
    dq exception3_handler
    dq exception4_handler
    dq exception5_handler
    dq exception6_handler
    dq exception7_handler
    dq exception8_handler
    dq exception9_handler
    dq exception10_handler
    dq exception11_handler
    dq exception12_handler
    dq exception13_handler
    dq exception14_handler
    dq exception15_handler
    dq exception16_handler
    dq exception17_handler
    dq exception18_handler
    dq exception19_handler
    dq exception20_handler
    dq exception21_handler
    dq exception22_handler
    dq exception23_handler
    dq exception24_handler
    dq exception25_handler
    dq exception26_handler
    dq exception27_handler
    dq exception28_handler
    dq exception29_handler
    dq exception30_handler
    dq exception31_handler
