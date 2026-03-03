#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <immintrin.h>
#include <cstdint>
#include "elias_gamma_packer.h"

int main(int argc, char ** args) {

    if(argc != 3)
    {
        std::cout << "Usage: rle_egp <input> <output>\n" << std::endl;
        return 1;
    }

    const char* filename = args[1];

    EliasGammaPacker egp;

    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        perror("fstat");
        close(fd);
        return 1;
    }

    if (sb.st_size == 0) {
        close(fd);
        return 0;
    }

    size_t fileSize = sb.st_size;

    void* map = mmap(nullptr, fileSize, PROT_READ, MAP_PRIVATE, fd, 0);
    if (map == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }

    posix_madvise(map, fileSize, MADV_SEQUENTIAL);

    uint8_t* data = static_cast<uint8_t*>(map);

    size_t bitPos = 0;

    uint8_t prevBit = (data[0] >> 7) & 1;
    size_t runLength = 0;

    size_t i = 0;

    // Process each bits
    for (; i < fileSize; ++i) {
        uint8_t b = data[i];
        for (int bit = 7; bit >= 0; --bit) {
            uint8_t currentBit = (b >> bit) & 1;

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

    munmap(map, fileSize);
    close(fd);

    egp.serialize(args[2]);

    return 0;
}
