%define E820_ADDR        0x4000
%define FIRST_PML4_BASE 0x32000
%define FIRST_PDPT_BASE 0x36000
%define FIRST_PD_BASE   0x37000
%define PAGE_PRESENT    (1 << 0)
%define PAGE_WRITE      (1 << 1)
%define PAGE_SIZE       (1 << 7)

[BITS 16]
[ORG 0x7c00]
_start:
    cli
    ; Set A20 line 
    in al, 0x92
    or al, 2
    out 0x92, al
    
    ; Set stack segment to 0
    xor dx, dx
    mov ss, dx
    mov esp, 0x7c00

    ; Get e820 map
    call get_physical
    
    ; Load GDTR
    xor ax, ax
    mov ds, ax
    lgdt [ds:gdt_desc]

    ; Set-up protected mode
    mov eax, cr0
    or eax, (0x1 << 0)
    mov cr0, eax
    ; Set data segment to 0x10
    xor dx, dx
    mov dx, 0x10
    mov ds, dx
    mov ss, dx
    mov es, dx
    mov fs, dx
    mov gs, dx
    ; Jump to protected mode !
    jmp 0x0008:entry

; Get physical memory map from E820
get_physical:
    ; 0x1000 is segment descriptor for es register
    mov bx, 0x1000
    mov es, bx

    ; Load e20 buffer
    xor ebx,ebx
    xor edi, edi
    mov di, E820_ADDR

    ; Entry counter initialization
    mov ebp, 0x0
L1:
    ; e820 is function code
    mov eax, 0xe820
    ; Each entry size is 24
    mov ecx, 24
    ; SMAP field
    mov edx, 0x534d4150
    ; Interrupt to query system address map 
    int 0x15
    
    ; Increment count of entries
    inc ebp
    
    ; Add offset to buffer address
    add edi, 24
    ; Compare ebx with continuation value
    cmp ebx, 0
    jnz L1

    ; Move count of entries to buffer to use in PXE kernel
    ; our kernel can read count value from 0x14000 (0x1000 * 16 + 0x4000)
    xor edx, edx
    mov edx, E820_ADDR ; offset in es segment address is 0x4000
    mov dword[es: edx + 0x200], ebp ; es segment address is 0x1000

    ret

[BITS 32]
entry:
    ; Initialize Page Table
    call early_page

    ; Disable IRQs
    mov al, 0xFF
    out 0xA1, al
    out 0x21, al
    lidt [idt]

    ; Set PAE flag
    mov eax, cr4
    or  eax, 1 << 5
    mov cr4, eax
    ; Set Page Directory address to cr3
    mov esi, FIRST_PML4_BASE
    mov cr3, esi

; Set-up Long Mode
setup_lm:
    
    ; Set NXE and LME
    mov edx, 0
    mov eax, 0x00000900
    mov ecx, 0xc0000080
    wrmsr
    mov eax, cr0
    or  eax,  (1 << 31)
    mov cr0, eax
    
    ; Load the 64-bit long mode GDT
    lgdt [lmgdt]
    ; Long jump to enable long mode!
    jmp 0x0008:enter_long

; Zeroing destination memory address
do_memset_zero:
    xor eax, eax
    rep stosd
    ret
   
; Fill Page Directory
fill_pd:
    xor eax, eax
    or  eax, PAGE_PRESENT
    or  eax, PAGE_WRITE
    or  eax, PAGE_SIZE
    or  eax, ebx
    mov dword[edi], eax
    add edi, 8
    add ebx, 0x200000 ; 2 MiB
    loop fill_pd
    ret

early_page:    
    ; Clear 1024 * 4 bytes in PML4
    mov edi, FIRST_PML4_BASE
    mov ecx, 4096 ; zeroing PML4, PDP, PD
    call do_memset_zero
    
    ; Initialize PML4 entry with present, write flags and PDPT base address
    xor eax, eax
    or  eax, PAGE_PRESENT
    or  eax, PAGE_WRITE
    or  eax, FIRST_PDPT_BASE
    mov edi, FIRST_PML4_BASE
    mov dword[edi], eax
    
    ; Initialize PDPT entry with present, write flags and PD base address
    xor eax, eax
    or  eax, PAGE_PRESENT
    or  eax, PAGE_WRITE
    or  eax, FIRST_PD_BASE
    mov edi, FIRST_PDPT_BASE
    mov dword[edi], eax

    ; Fill Page Directory
    xor ebx, ebx
    mov ecx, 512
    mov edi, FIRST_PD_BASE
    call fill_pd
    ret


align 4
idt:
    dw 0 ; Len
    dd 0 ; Base

align 8
gdt_base:
    dq 0x0000000000000000
    dq 0x00cf9a000000ffff
    dq 0x00cf92000000ffff

gdt_desc:
        dw (gdt_desc - gdt_base) - 1
        dd gdt_base

; 64-bit long mode GDT

align 8
lmgdt_base:
    dq 0x0000000000000000 ; Null descriptor
    dq 0x00209a0000000000 ; 64-bit, present, code
    dq 0x0000920000000000 ; Present, data r/w

lmgdt:
    dw (lmgdt - lmgdt_base) - 1
    dd lmgdt_base
    dd 0

[bits 64]
enter_long:
    mov rsp, 0x50000
    jmp entry_p

times 510 - ($-$$) db 0
dw 0xaa55

entry_p:
    incbin "../../elf_parse/flat.bin"
