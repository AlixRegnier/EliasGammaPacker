#include "bitpacker.h"

#include <cstdint>
#include <stdexcept>
#include <bits/stdc++.h>
#include <vector>

#define LOG_FORBIDDEN_VALUE 255
const std::uint8_t BitPacker::tab64[] = {
    LOG_FORBIDDEN_VALUE,  0, 58,  1, 59, 47, 53,  2,
                     60, 39, 48, 27, 54, 33, 42,  3,
                     61, 51, 37, 40, 49, 18, 28, 20,
                     55, 30, 34, 11, 43, 14, 22,  4,
                     62, 57, 46, 52, 38, 26, 32, 41,
                     50, 36, 17, 19, 29, 10, 13, 21,
                     56, 45, 25, 31, 35, 16,  9, 12,
                     44, 24, 15,  8, 23,  7,  6,  5
};

//255 = exception value
const std::uint8_t BitPacker::tab8[] = {
    LOG_FORBIDDEN_VALUE,7,6,6,5,5,5,5,
                      4,4,4,4,4,4,4,4,
                      3,3,3,3,3,3,3,3,
                      3,3,3,3,3,3,3,3,
                      2,2,2,2,2,2,2,2,
                      2,2,2,2,2,2,2,2,
                      2,2,2,2,2,2,2,2,
                      2,2,2,2,2,2,2,2,
                      1,1,1,1,1,1,1,1,
                      1,1,1,1,1,1,1,1,
                      1,1,1,1,1,1,1,1,
                      1,1,1,1,1,1,1,1,
                      1,1,1,1,1,1,1,1,
                      1,1,1,1,1,1,1,1,
                      1,1,1,1,1,1,1,1,
                      1,1,1,1,1,1,1,1,
                      0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0
};

void BitPacker::printByte(std::uint8_t byte)
{
    for(int i = 0; i < 8; ++i)
    {
        if(byte >> (7-i) & std::uint8_t{1})
            std::cout << '1';
        else
            std::cout << '0';
    }
}

// Pack a value with specified number of bits
void BitPacker::pack(std::uint64_t value, std::size_t num_bits) {
    if (num_bits == 0 || num_bits > 64) {
        throw std::invalid_argument("BitPacker::pack : num_bits must be between 1 and 64");
    }

    // Ensure value fits in num_bits
    if (num_bits < 64 && value >= (std::uint64_t{1} << num_bits)) {
        throw std::invalid_argument("BitPacker::pack : value too large for specified number of bits");
    }

    ++packed_values;

    std::size_t byte_idx = bit_position / 8;
    std::size_t bit_offset = bit_position % 8;
    std::size_t bits_remaining = num_bits;

    // Ensure there is enough place, increase size by 50% if not
    std::size_t bytes_needed = (bit_position + num_bits + 7) / 8;
    if (data.size() < bytes_needed)
        data.resize(std::max(data.size()*3/2, bytes_needed));

    // If we're not byte-aligned, handle the first partial byte
    if (bit_offset != 0) {
        std::size_t bits_in_first_byte = std::min(8 - bit_offset, bits_remaining);
        std::uint8_t mask = (1 << bits_in_first_byte) - 1;
        std::uint8_t bits_to_pack = (value >> (bits_remaining - bits_in_first_byte)) & mask;

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
        std::uint8_t mask = (1 << bits_remaining) - 1;
        std::uint8_t bits_to_pack = value & mask;
        data[byte_idx] |= bits_to_pack << (8 - bits_remaining);
    }

    bit_position += num_bits;
}

// Unpack a value with specified number of bits
std::uint64_t BitPacker::unpack(std::size_t start_bit, std::size_t num_bits) const {
    if (num_bits == 0 || num_bits > 64) {
        throw std::invalid_argument("BitPacker::unpack : num_bits must be between 1 and 64");
    }

    if (start_bit + num_bits > bit_position) {
        throw std::out_of_range("BitPacker::unpack : attempting to read beyond packed data");
    }

    std::uint64_t result = 0;
    std::size_t byte_idx = start_bit / 8;
    std::size_t bit_offset = start_bit % 8;
    std::size_t bits_remaining = num_bits;

    // Handle first partial byte if not aligned
    if (bit_offset != 0) {
        std::size_t bits_in_first_byte = std::min(8 - bit_offset, bits_remaining);
        std::uint8_t mask = (1 << bits_in_first_byte) - 1;
        std::uint8_t bits = (data[byte_idx] >> (8 - bit_offset - bits_in_first_byte)) & mask;

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
        std::uint8_t bits = data[byte_idx] >> (8 - bits_remaining);
        result = (result << bits_remaining) | bits;
    }

    return result;
}

std::size_t BitPacker::get_next_one_pos(std::size_t starting_bit_pos) const
{
    const std::uint8_t mask = ~std::uint8_t{0};

    //Mask bits before bit starting position
    const std::uint8_t masked_byte = data[starting_bit_pos/8] & (mask >> (starting_bit_pos % 8));

    //Return first 1 position in masked byte if any + offset
    if(masked_byte != 0)
        return log2_8(masked_byte) + (starting_bit_pos/8*8);

    for(std::size_t i = starting_bit_pos/8+1; i < data.size(); ++i)
    {
        if(data[i] != 0)
            return log2_8(data[i]) + i*8;
    }

    throw std::runtime_error("BitPacker::get_next_one_pos : Couldn't find any more one from this position");
}

void BitPacker::print() const
{
    for(std::size_t i = 0; i < data.size(); ++i)
    {
        printByte(data[i]);
        std::cout << ' ';
    }

    std::cout << std::endl;
}

void BitPacker::serialize(const std::string& output_file) const
{
    std::ofstream f(output_file, std::ofstream::binary);

    if(!f.is_open())
    {
        f.close();
        throw std::runtime_error("BitPacker::serialize : couldn't open file '" + output_file + "'");
    }

    //Serialize bit position
    f.write(reinterpret_cast<const char*>(&bit_position), sizeof(bit_position));

    //Serialize number of packed values
    f.write(reinterpret_cast<const char*>(&packed_values), sizeof(packed_values));

    //Serialize payload
    const std::size_t payload_size = (bit_position + 7) / 8;
    f.write(reinterpret_cast<const char*>(data.data()), payload_size);

    f.close();
}

void BitPacker::deserialize(const std::string& input_file)
{
    std::ifstream f(input_file, std::ifstream::binary);

    if(!f.is_open())
    {
        f.close();
        throw std::runtime_error("BitPacker::deserialize : couldn't open file '" + input_file + "'");
    }
    
    //Deserialize bit position (number of bits)
    f.read(reinterpret_cast<char*>(&bit_position), sizeof(bit_position));

    //Deserialize number of packed values
    f.read(reinterpret_cast<char*>(&packed_values), sizeof(packed_values));

    data.resize((bit_position+7)/8);

    if(!f.read(reinterpret_cast<char*>(data.data()), data.size()))
    {
        f.close();
        throw std::runtime_error("BitPacker::deserialize : unexpected file size");
    }

    f.close();
}

BitPacker::~BitPacker(){}