[BITS 16]
[ORG 0x7c00]
_start:
    cli
    cld
    ;set new line

    ;set A20 line 
    in al, 0x92
    or al, 2
    out 0x92, al
    
    ;load GDTR
    xor ax, ax
    mov ds, ax
    lgdt [ds:gdt_desc]

    ;Setting up protected mode
    mov eax, cr0
    or eax, (0x1 << 0)
    mov cr0, eax
    ;set data segment to 0x10
    mov dx, 0x10
    mov ds, dx
    mov ss, dx
    mov es, dx
    mov fs, dx
    mov gs, dx
    
    
    jmp 0x0008:entry

[BITS 32]

entry:
    mov esp, 0x7c00
    jmp entry_p

align 8
gdt_base:
    dq 0x0000000000000000
    dq 0x00cf9a000000ffff
    dq 0x00cf92000000ffff

gdt_desc:
        dw (gdt_desc - gdt_base) - 1
        dd gdt_base

times 510 - ($-$$) db 0
dw 0xaa55

entry_p:
    incbin "../../elf_parse/flat.bin"
