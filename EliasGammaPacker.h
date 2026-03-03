#ifndef ELIAS_GAMMA_PACKER_H
#define ELIAS_GAMMA_PACKER_H

#include <cstdint>
#include <stdexcept>
#include <bits/stdc++.h>
#include <vector>

class BitPacker {
protected:
    std::vector<uint8_t> data;
    size_t bit_position{0};  // Current bit position in the data vector

    static const std::uint8_t tab64[64];
    static const std::uint8_t tab8[8];

    static std::uint8_t log2_64 (uint64_t value)
    {
        value |= value >> 1;
        value |= value >> 2;
        value |= value >> 4;
        value |= value >> 8;
        value |= value >> 16;
        value |= value >> 32;
        return tab64[((uint64_t)((value - (value >> 1))*0x07EDD5E59A4E28C2)) >> 58];
    }

    static std::uint8_t log2_8(std::uint8_t value)
    {
        value |= value >> 1;
        value |= value >> 2;
        value |= value >> 4;
        return tab8[((uint8_t)((value - (value >> 1))*0x1D)) >> 5];
    }

    void printByte(std::uint8_t byte)
    {
        for(int i = 0; i < 8; ++i)
        {
            if(byte >> (7-i) & std::uint8_t{1})
                std::cout << '1';
            else
                std::cout << '0';
        }
    }
public:
    BitPacker() = default;

    // Pack a value with specified number of bits
    void pack(uint64_t value, size_t num_bits) {
        if (num_bits == 0 || num_bits > 64) {
            throw std::invalid_argument("num_bits must be between 1 and 64");
        }

        // Ensure value fits in num_bits
        if (num_bits < 64 && value >= (1ULL << num_bits)) {
            throw std::invalid_argument("value too large for specified number of bits");
        }

        size_t byte_idx = bit_position / 8;
        size_t bit_offset = bit_position % 8;
        size_t bits_remaining = num_bits;

        // Ensure we have enough space
        size_t bytes_needed = (bit_position + num_bits + 7) / 8;
        if (data.size() < bytes_needed) {
            data.resize(bytes_needed, 0);
        }

        // If we're not byte-aligned, handle the first partial byte
        if (bit_offset != 0) {
            size_t bits_in_first_byte = std::min(8 - bit_offset, bits_remaining);
            uint8_t mask = (1 << bits_in_first_byte) - 1;
            uint8_t bits_to_pack = (value >> (bits_remaining - bits_in_first_byte)) & mask;

            data[byte_idx] |= bits_to_pack << (8 - bit_offset - bits_in_first_byte);

            bits_remaining -= bits_in_first_byte;
            byte_idx++;
        }

        // Pack full bytes
        while (bits_remaining >= 8) {
            bits_remaining -= 8;
            data[byte_idx] = (value >> bits_remaining) & 0xFF;
            byte_idx++;
        }

        // Pack remaining bits (less than 8)
        if (bits_remaining > 0) {
            uint8_t mask = (1 << bits_remaining) - 1;
            uint8_t bits_to_pack = value & mask;
            data[byte_idx] |= bits_to_pack << (8 - bits_remaining);
        }

        bit_position += num_bits;
    }

    // Unpack a value with specified number of bits
    uint64_t unpack(size_t start_bit, size_t num_bits) const {
        if (num_bits == 0 || num_bits > 64) {
            throw std::invalid_argument("num_bits must be between 1 and 64");
        }

        if (start_bit + num_bits > bit_position) {
            throw std::out_of_range("attempting to read beyond packed data");
        }

        uint64_t result = 0;
        size_t byte_idx = start_bit / 8;
        size_t bit_offset = start_bit % 8;
        size_t bits_remaining = num_bits;

        // Handle first partial byte if not aligned
        if (bit_offset != 0) {
            size_t bits_in_first_byte = std::min(8 - bit_offset, bits_remaining);
            uint8_t mask = (1 << bits_in_first_byte) - 1;
            uint8_t bits = (data[byte_idx] >> (8 - bit_offset - bits_in_first_byte)) & mask;

            result = bits;
            bits_remaining -= bits_in_first_byte;
            byte_idx++;
        }

        // Read full bytes
        while (bits_remaining >= 8) {
            result = (result << 8) | data[byte_idx];
            bits_remaining -= 8;
            byte_idx++;
        }

        // Read remaining bits
        if (bits_remaining > 0) {
            uint8_t bits = data[byte_idx] >> (8 - bits_remaining);
            result = (result << bits_remaining) | bits;
        }

        return result;
    }

    // Get the underlying data
    const std::vector<uint8_t>& get_data() const {
        return data;
    }

    // Get the number of bits packed
    size_t get_bit_count() const {
        return bit_position;
    }

    // Get the number of bytes used (rounded up)
    size_t get_byte_count() const {
        return data.size();
    }

    std::size_t get_next_one_pos(std::size_t starting_bit_pos)
    {
        const std::uint8_t mask = ~std::uint8_t{0};

        //Mask bits before bit starting position
        const std::uint8_t masked_byte = data[starting_bit_pos/8] & (mask >> (starting_bit_pos % 8));

        std::uint8_t l = log2_8(masked_byte);

        //Return first 1 position in masked byte if any
        if(l || (masked_byte & std::uint8_t{1})) //If a one occurs in first byte
            return 7 - l + starting_bit_pos/8*8;

        for(std::size_t i = starting_bit_pos/8+1; i < data.size(); ++i)
        {
            l = log2_8(data[i]);

            if(l || (data[i] & std::uint8_t{1}))
                return 7 - l + i*8;
        }

        throw std::runtime_error("Couldn't find any more 1");
    }

    // Clear all data
    void clear() {
        data.clear();
        bit_position = 0;
    }

    // Reset to allow repacking from the beginning
    void reset() {
        bit_position = 0;
        std::fill(data.begin(), data.end(), 0);
    }

    void print()
    {
        for(std::size_t i = 0; i < data.size(); ++i)
        {
            printByte(data[i]);
            std::cout << ' ';
        }

        std::cout << std::endl;
    }

    void serialize(const std::string& output_file) const
    {
        std::ofstream f(output_file, std::ofstream::binary);

        for(std::size_t i = 0; i < data.size(); ++i)
            f << data[i];

        f.close();
    }

    void deserialize(const std::string& input_file)
    {
        std::ifstream f(input_file, std::ifstream::binary | std::ifstream::ate);

        std::streamsize size = f.tellg();  // Get position (size)

        f.seekg(0, std::ios::beg); //Reset cursor to beginning

        if(size <= 0)
            throw std::runtime_error("File size is incorrect");

        data.resize(size);

        if(!f.read(reinterpret_cast<char*>(data.data()), size))
        {
            f.close();
            throw std::runtime_error("Couldn't read file");
        }
        bit_position = size*8;
        f.close();
    }

    virtual ~BitPacker() {}
};

class EliasGammaPacker : public BitPacker
{
private:
    size_t next_elias_position{0};

    static std::uint64_t elias_delta_bit_length(std::uint64_t x)
    {
        return 2*log2_64(x)+1;
    }
public:
    EliasGammaPacker() = default;

    void pack(std::uint64_t value)
    {
        //std::cout << value << " " << elias_delta_bit_length(value+1) << std::endl;
        BitPacker::pack(value, elias_delta_bit_length(value));
    }

    std::uint64_t unpack()
    {
        std::size_t a = next_elias_position;
        std::size_t b = get_next_one_pos(next_elias_position);

        b += b - a;
        next_elias_position = b+1;
        return BitPacker::unpack(a, b-a+1);
    }
};

const std::uint8_t BitPacker::tab64[] = {
    0,  0, 58,  1, 59, 47, 53,  2,
    60, 39, 48, 27, 54, 33, 42,  3,
    61, 51, 37, 40, 49, 18, 28, 20,
    55, 30, 34, 11, 43, 14, 22,  4,
    62, 57, 46, 52, 38, 26, 32, 41,
    50, 36, 17, 19, 29, 10, 13, 21,
    56, 45, 25, 31, 35, 16,  9, 12,
    44, 24, 15,  8, 23,  7,  6,  5
};

const std::uint8_t BitPacker::tab8[] = {
    0, 1, 6, 2, 7, 5, 4, 3
};

#endif
