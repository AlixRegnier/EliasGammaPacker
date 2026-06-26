#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdint>
#include <string>
#include "egprle.h"

int main(int argc, char ** args) {

    if(argc != 3)
    {
        std::cout << "Usage: rle_egp <input> <output>\n" << std::endl;
        return 1;
    }

    std::string in_filename = args[1];
    std::string out_filename = args[2];

    if(in_filename == out_filename)
    {
        std::cerr << "main (encoder) : input and output can't point to same file" << std::endl;
        return 2;
    }

    //Input file
    int in_fd = open(in_filename.c_str(), O_RDONLY);

    if (in_fd == -1)
    {
        std::cerr << "main (encoder) : couldn't open file '" << in_filename << "' (" << strerror(errno) << ')' << std::endl;
        return 2;
    }

    std::size_t in_file_size = lseek(in_fd, 0, SEEK_END);

    if(in_file_size == 0)
    {
        std::cerr << "main : input file size is 0" << std::endl;
        return 2;
    }

    char* in_map = (char*)mmap(nullptr, in_file_size, PROT_READ, MAP_PRIVATE, in_fd, 0);
    
    if (in_map == MAP_FAILED)
    {
        std::cerr << "main : mmap initialization failed (" << strerror(errno) << ')' << std::endl;
        return 2;
    }

    posix_madvise(in_map, in_file_size, MADV_SEQUENTIAL);

    //Output file
    int out_fd = open(out_filename.c_str(), O_TRUNC | O_CREAT | O_RDWR, 0644);

    if (out_fd == -1)
    {
        std::cerr << "main : couldn't open file '" << out_filename << "' (" << strerror(errno) << ')' << std::endl;
        return 2;
    }

    //Estimated size
    std::size_t out_file_size = EliasGammaPacker::EGPRLE::compressBound(in_file_size);

    if (ftruncate(out_fd, out_file_size) == -1) 
    {
        std::cerr << "main : file resizing failed (" << strerror(errno) << ')' << std::endl;
        return 2;
    }

    char* out_map = (char*)mmap(nullptr, out_file_size, PROT_READ | PROT_WRITE, MAP_SHARED, out_fd, 0);

    if (out_map == MAP_FAILED)
    {
        std::cerr << "main : mmap initialization failed (" << strerror(errno) << ')' << std::endl;
        return 2;
    }
    
    posix_madvise(out_map, out_file_size, MADV_SEQUENTIAL);

    //Encode
    out_file_size = EliasGammaPacker::EGPRLE().encode(out_map, out_file_size, in_map, in_file_size);

    if (ftruncate(out_fd, out_file_size) == -1)
    {
        std::cerr << "main : file resizing failed (" << strerror(errno) << ')' << std::endl;
        return 2;
    }

    munmap(in_map, in_file_size);
    munmap(out_map, out_file_size);
    close(in_fd);
    close(out_fd);

    return 0;
}
