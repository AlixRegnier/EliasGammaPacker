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

#include "EliasGammaPacker.h"


int main(int argc, char ** args) {

    if(argc != 3)
    {
        std::cout << "Usage: unrle_egc <input> <output>\n" << std::endl;
        return 1;
    }

    std::string input = args[1];
    std::string output = args[2];

    EliasGammaPacker egp;
    egp.deserialize(input);

    std::ofstream f(output, std::ofstream::binary);

    constexpr size_t BUFFER_SIZE = 1 << 23; // 8MB
    std::vector<uint8_t> buffer(BUFFER_SIZE);
    size_t buffer_pos = 0;

    auto flush_buffer = [&]() {
        f.write(reinterpret_cast<char*>(buffer.data()), buffer_pos);
        buffer_pos = 0;
    };

    auto write_byte = [&](uint8_t byte) {
        if (buffer_pos >= BUFFER_SIZE)
            flush_buffer();

        buffer[buffer_pos++] = byte;
    };

    uint8_t current_byte = 0;
    int bit_pos = 7;              // MSB-first
    bool bit_value = false;       // start with 0-run
    std::uint64_t run_length;
    try
    {
	    while(true)
	    {
	        run_length = egp.unpack();

	        // Handle unaligned partial byte first
	        while (run_length > 0 && bit_pos != 7)
	        {
	            if (bit_value)
	                current_byte |= (1u << bit_pos);

	            bit_pos--;
	            run_length--;

	            if (bit_pos < 0)
	            {
	                write_byte(current_byte);
	                current_byte = 0;
	                bit_pos = 7;
	            }
	        }

	        // Now we are byte-aligned
	        if (run_length >= 8)
	        {
	            uint8_t fill_byte = bit_value ? 0xFF : 0x00;

	            // Number of full bytes
	            uint64_t full_bytes = run_length / 8;
	            run_length %= 8;

	            // Emit 32-byte AVX blocks
	            __m256i vec = _mm256_set1_epi8(static_cast<char>(fill_byte));

	            while (full_bytes >= 32)
	            {
	                if (buffer_pos + 32 > BUFFER_SIZE)
	                    flush_buffer();

	                _mm256_storeu_si256(
	                    reinterpret_cast<__m256i*>(buffer.data() + buffer_pos),
	                    vec);

	                buffer_pos += 32;
	                full_bytes -= 32;
	            }

	            // Emit remaining full bytes
	            while (full_bytes--)
	                write_byte(fill_byte);
	        }

	        // Handle tail bits (< 8)
	        while (run_length > 0)
	        {
	            if (bit_value)
	                current_byte |= (1u << bit_pos);

	            bit_pos--;
	            run_length--;

	            if (bit_pos < 0)
	            {
	                write_byte(current_byte);
	                current_byte = 0;
	                bit_pos = 7;
	            }
	        }

	        bit_value = !bit_value;
	    }
    }
    catch(std::runtime_error& e){}

    // Flush partial byte
    if (bit_pos != 7)
        write_byte(current_byte);

    // Flush remaining buffer
    if (buffer_pos)
        flush_buffer();

    f.close();
    return 0;
}
