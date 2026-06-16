#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <immintrin.h>
#include <cstdint>
#include <stdexcept>
#include <fstream>
#include <vector>
#include <cstring>

#include "egprle.h"

int main(int argc, char ** args) {

    if(argc != 3)
    {
        std::cout << "Usage: unrle_egp <input> <output>\n" << std::endl;
        return 1;
    }

    std::string in_filename = args[1];
    std::string out_filename = args[2];

    //Input file
    int in_fd = open(in_filename.c_str(), O_RDWR);

    if (in_fd == -1)
    {
        std::cerr << "main : couldn't open file '" << in_filename << "' (" << strerror(errno) << ')' << std::endl;
        return 2;
    }

    std::size_t in_file_size = lseek(in_fd, 0, SEEK_END);

    char* in_map = (char*)mmap(nullptr, in_file_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, in_fd, 0);

    if (in_map == MAP_FAILED)
    {
        std::cerr << "main : mmap initialization failed from '" << in_filename << "' (" << strerror(errno) << ')' << std::endl;

        close(in_fd);
        return 2;
    }

    posix_madvise(in_map, in_file_size, MADV_SEQUENTIAL);

    //Output file
    int out_fd = open(out_filename.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0644);

    if (out_fd == -1)
    {
        std::cerr << "main : couldn't open file '" << out_filename << "' (" << strerror(errno) << ')' << std::endl;
        munmap(in_map, in_file_size);
        close(in_fd);
        return 2;
    }

    std::size_t out_file_size = EliasGammaPacker::EGPRLE::read_meta(in_map).out_size;
    if (ftruncate(out_fd, out_file_size) == -1)
    {
        std::cerr << "main : file resizing failed (" << strerror(errno) << ')' << std::endl;
        munmap(in_map, in_file_size);
        close(in_fd);
        close(out_fd);
        return 2;
    }

    char* out_map = (char*)mmap(nullptr, out_file_size, PROT_READ | PROT_WRITE, MAP_SHARED, out_fd, 0);

    if (out_map == MAP_FAILED)
    {
        std::cerr << "main : mmap initialization failed from '" << out_filename << "' (" << strerror(errno) << ')' << std::endl;
        munmap(in_map, in_file_size);
        close(in_fd);
        close(out_fd);
        return 2;
    }

    posix_madvise(out_map, out_file_size, MADV_SEQUENTIAL);

    std::size_t written_bytes = EliasGammaPacker::EGPRLE().decode(out_map, out_file_size, in_map, in_file_size);

    if(written_bytes != out_file_size)
    {
        std::cerr << "Unexpected out file size, written '" << written_bytes << "' instead of '" << out_file_size << '\'' << std::endl;
        munmap(in_map, in_file_size);
        munmap(out_map, out_file_size);
        close(in_fd);
        close(out_fd);
        return 2;
    }

    munmap(in_map, in_file_size);
    munmap(out_map, out_file_size);
    close(in_fd);
    close(out_fd);

    return 0;
}
