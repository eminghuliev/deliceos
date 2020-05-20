extern "C"
{
    extern void (**__init_array_start)();
    extern void (**__init_array_end)();

    //This function will find all global definition calls 
    //by iterating over __init_array_start section and call them.
    inline void static_init()
    {
        for (void (**p)() = __init_array_start; p < __init_array_end; ++p)
            (*p)();
    }
}

extern "C" void start_kernel () __attribute__ ((section ("entry")));
