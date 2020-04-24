#include <file.h>
File::File(const char* filename){
    file.open(filename, std::ios_base::binary);
}

File::~File(){
    file.close();
}

uint8_t* File::openfile(const char* file) {
    
    int fd;
    uint8_t *addr;
    fd = open(file, O_RDONLY);
    assert (fd > -1);
    off_t fsize;
    //Retrieve file size
    fsize = lseek(fd, 0, SEEK_END);
    addr = static_cast<uint8_t*> 
        (mmap(NULL, fsize, PROT_READ, MAP_PRIVATE, fd, 0)); 
    close(fd);
    return addr;
}


void File::writefile(uint8_t *byte){
    file.write((char*)byte, sizeof(uint8_t));
}
