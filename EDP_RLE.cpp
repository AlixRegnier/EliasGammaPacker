#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <immintrin.h>
#include <cstdint>
#include "EliasDeltaPacker.h"

int main(int argc, char ** args) {
    const char* filename = args[1];

    EliasDeltaPacker edp;

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

    size_t totalBits = fileSize * 8;
    size_t bitPos = 0;

    uint8_t prevBit = (data[0] >> 7) & 1;
    size_t runLength = 0;

    size_t i = 0;

#ifdef __AVX2__
    const size_t simdWidth = 32;

    for (; i + simdWidth <= fileSize; i += simdWidth) {
        __m256i block = _mm256_loadu_si256((__m256i*)(data + i));

        // Shift each byte left by 1
        __m256i shifted = _mm256_slli_epi16(block, 1);

        // XOR detects bit transitions within each byte
        __m256i transitions = _mm256_xor_si256(block, shifted);

        // Extract MSB of each bit position
        uint32_t mask = _mm256_movemask_epi8(transitions);

        for (int byte = 0; byte < 32; ++byte) {
            uint8_t b = data[i + byte];

            for (int bit = 7; bit >= 0; --bit) {
                uint8_t currentBit = (b >> bit) & 1;

                if (currentBit == prevBit) {
                    ++runLength;
                } else {
                    edp.pack(runLength);
                    runLength = 1;
                    prevBit = currentBit;
                }
                ++bitPos;
            }
        }
    }
#endif

    // Scalar tail
    for (; i < fileSize; ++i) {
        uint8_t b = data[i];
        for (int bit = 7; bit >= 0; --bit) {
            uint8_t currentBit = (b >> bit) & 1;

            if (currentBit == prevBit) {
                ++runLength;
            } else {
                edp.pack(runLength);
                runLength = 1;
                prevBit = currentBit;
            }
            ++bitPos;
        }
    }

    // Print final run
    if (runLength > 0) {
        edp.pack(runLength);
    }

    munmap(map, fileSize);
    close(fd);

    edp.dump(args[2]);

    return 0;
}
