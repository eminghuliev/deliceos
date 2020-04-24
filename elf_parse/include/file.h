#include <sys/stat.h>
#include <fcntl.h>
#include <ostream>
#include <sys/mman.h>
#include <unistd.h>
#include <cstdint>
#include <elf.h>
#include <fstream>
#include <cassert>
class File {
    public:
        File(const char* filename);
        ~File();
        uint8_t* openfile(const char* file);
        void writefile(uint8_t *byte);
    private:
        std::ofstream file;
};

