#include <cstdio>
#include <file.h>
#include <algorithm>
#include <vector>
#include <iostream>

typedef struct FileHeader {
    uint32_t offset;
    uint32_t size;
    FileHeader(uint32_t f_offset, uint32_t f_size)
        : offset(f_offset), size(f_size){};
} initheader;

bool comparator(const FileHeader& s1, const FileHeader& s2) {
    return s1.offset < s2.offset;
}
