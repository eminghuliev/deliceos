ENTRY(start_kernel);
SECTIONS
{    
    . = 0x8100;
    .text : AT(0x8100)
    {
        _text = .;
        *(entry)
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

