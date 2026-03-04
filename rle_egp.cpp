#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <immintrin.h>
#include <cstdint>
#include <string>
#include "elias_gamma_packer.h"

int main(int argc, char ** args) {

    if(argc != 3)
    {
        std::cout << "Usage: rle_egp <input> <output>\n" << std::endl;
        return 1;
    }

    std::string filename = args[1];

    EliasGammaPacker egp;

    int fd = open(filename.c_str(), O_RDONLY);
    if (fd == -1)
        throw std::runtime_error("main : couldn't open file '" + filename + "'");


    std::size_t file_size = lseek(fd, 0, SEEK_END);

    void* map = mmap(nullptr, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (map == MAP_FAILED) {
        throw std::runtime_error("main : mmap initialization failed");
    }

    posix_madvise(map, file_size, MADV_SEQUENTIAL);

    std::uint8_t* data = static_cast<std::uint8_t*>(map);

    std::size_t bitPos = 0;    
    std::size_t runLength = 0;
    std::size_t i = 0;

    std::uint8_t prevBit = (data[0] >> 7) & 1;
    egp.set_first_bit(prevBit);

    // Process each bits
    for (; i < file_size; ++i) {
        std::uint8_t b = data[i];
        for (int bit = 7; bit >= 0; --bit) {
            std::uint8_t currentBit = (b >> bit) & 1;

            if (currentBit == prevBit) {
                ++runLength;
            } else {
                egp.pack(runLength);
                runLength = 1;
                prevBit = currentBit;
            }
            ++bitPos;
        }
    }

    // Print final run
    if (runLength > 0) {
        egp.pack(runLength);
    }

    munmap(map, file_size);
    close(fd);

    egp.serialize(args[2]);

    return 0;
}
