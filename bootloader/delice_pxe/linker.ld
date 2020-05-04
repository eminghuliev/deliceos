ENTRY(start_kernel);
SECTIONS
{    
    . = 0x7e00;
    .text : AT(0x7e00)
    {
        _text = .;
        *(.text);
        _text_end = .;
    }
    .data :
    {
        _data = .;
        *(.bss);
        *(.bss*);
        *(.data);
        *(.rodata*);
        *(COMMON)
        _data_end = .;
    }
    . = ALIGN(4);
    .init_array :
    {
    __init_array_start = .;
    KEEP (*(.init_array*))
    __init_array_end = .;
    }
    
    /DISCARD/ :
    {
        *(.note*);
        *(.iplt*);
        *(.igot*);
        *(.rel*);
        *(.debug*);
        *(.comment);
    }
}

