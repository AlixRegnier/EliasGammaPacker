#include "bitpacker.h"

#include <cstdint>
#include <stdexcept>
#include <bits/stdc++.h>
#include <vector>

const std::uint8_t BitPacker::tab64[] = {
    63, 0, 58,  1, 59, 47, 53,  2,
    60, 39, 48, 27, 54, 33, 42,  3,
    61, 51, 37, 40, 49, 18, 28, 20,
    55, 30, 34, 11, 43, 14, 22,  4,
    62, 57, 46, 52, 38, 26, 32, 41,
    50, 36, 17, 19, 29, 10, 13, 21,
    56, 45, 25, 31, 35, 16,  9, 12,
    44, 24, 15,  8, 23,  7,  6,  5
};

//255 = exception value
#define NA 255
const std::uint8_t BitPacker::tab8[] = {
    NA, 0, 1, 1, 2, 2, 2, 2,
     3, 3, 3, 3, 3, 3, 3, 3,
     4, 4, 4, 4, 4, 4, 4, 4,
     4, 4, 4, 4, 4, 4, 4, 4,
     5, 5, 5, 5, 5, 5, 5, 5,
     5, 5, 5, 5, 5, 5, 5, 5,
     5, 5, 5, 5, 5, 5, 5, 5,
     5, 5, 5, 5, 5, 5, 5, 5,
     6, 6, 6, 6, 6, 6, 6, 6,
     6, 6, 6, 6, 6, 6, 6, 6,
     6, 6, 6, 6, 6, 6, 6, 6,
     6, 6, 6, 6, 6, 6, 6, 6,
     6, 6, 6, 6, 6, 6, 6, 6,
     6, 6, 6, 6, 6, 6, 6, 6,
     6, 6, 6, 6, 6, 6, 6, 6,
     6, 6, 6, 6, 6, 6, 6, 6,
     7, 7, 7, 7, 7, 7, 7, 7,
     7, 7, 7, 7, 7, 7, 7, 7,
     7, 7, 7, 7, 7, 7, 7, 7,
     7, 7, 7, 7, 7, 7, 7, 7,
     7, 7, 7, 7, 7, 7, 7, 7,
     7, 7, 7, 7, 7, 7, 7, 7,
     7, 7, 7, 7, 7, 7, 7, 7,
     7, 7, 7, 7, 7, 7, 7, 7,
     7, 7, 7, 7, 7, 7, 7, 7,
     7, 7, 7, 7, 7, 7, 7, 7,
     7, 7, 7, 7, 7, 7, 7, 7,
     7, 7, 7, 7, 7, 7, 7, 7,
     7, 7, 7, 7, 7, 7, 7, 7,
     7, 7, 7, 7, 7, 7, 7, 7,
     7, 7, 7, 7, 7, 7, 7, 7,
     7, 7, 7, 7, 7, 7, 7, 7
};
#undef NA

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
    if (num_bits == 0 || num_bits > sizeof(std::uint64_t)*8) {
        throw std::invalid_argument("BitPacker::pack : num_bits must be between 1 and 64");
    }

    // Ensure value fits in num_bits
    if (num_bits < sizeof(std::uint64_t)*8 && value >= (std::uint64_t{1} << num_bits)) {
        throw std::invalid_argument("BitPacker::pack : value too large for specified number of bits");
    }

    ++packed_values;

    constexpr unsigned log64 = 6;
    constexpr unsigned mod64 = 63;
    constexpr unsigned siz64 = 8*sizeof(std::uint64_t);

    const std::size_t p_index =  bit_position >> log64; //divide by (8*sizeof(std::uint64_t))
    const std::size_t p_offset = bit_position  & mod64; //mod    by (8*sizeof(std::uint64_t))

    // // Ensure there is enough place, increase size by 50% if not
    // std::size_t bytes_needed = (bit_position + num_bits + sizeof(std::uint64_t)*8 - 1) / (8*sizeof(std::uint64_t));
    // if (data.size() < bytes_needed)
    //     data.resize(std::max(data.size()*3/2, bytes_needed));

    //const int shift = (sizeof(std::uint64_t)*8 - num_bits - p_offset);

    std::uint64_t left  = (value << (siz64 - p_offset - num_bits));
    std::uint64_t right = (value >> (p_offset + num_bits - siz64));

    data[p_index]     |= left;
    data[p_index + 1] |= right;

    bit_position += num_bits;
}

// Unpack a value with specified number of bits
std::uint64_t BitPacker::unpack(std::size_t start_bit, std::size_t num_bits) const {
    if (num_bits == 0 || num_bits > sizeof(std::uint64_t)*8) {
        throw std::invalid_argument("BitPacker::unpack : num_bits must be between 1 and 64");
    }

    if (start_bit + num_bits > bit_position) {
        throw std::out_of_range("BitPacker::unpack : attempting to read beyond packed data");
    }

    constexpr unsigned log64 = 6;
    constexpr unsigned mod64 = 63;
    constexpr unsigned siz64 = 8*sizeof(std::uint64_t);

    const std::size_t p_index =  bit_position >> log64; //divide by (8*sizeof(std::uint64_t))
    const std::size_t p_offset = bit_position  & mod64; //mod    by (8*sizeof(std::uint64_t))

     __uint128_t window =
        (static_cast<__uint128_t>(data[p_index]) << siz64) |
        data[p_index + 1];

    std::size_t shift = 2*siz64 - p_offset - num_bits;

    std::uint64_t result = static_cast<std::uint64_t>(window >> shift);

    if (num_bits < siz64)
        result &= ((std::uint64_t{1} << num_bits) - 1);
    
    return result;
}

std::size_t BitPacker::get_next_one_pos(std::size_t starting_bit_pos) const
{
    const std::uint8_t * const data8_vec = reinterpret_cast<const std::uint8_t* const>(data.data());
    const std::size_t data8_size = (bit_position+7)/8;

    const std::uint8_t mask = ~std::uint8_t{0};

    //Mask bits before bit starting position
    const std::uint8_t masked_byte = data8_vec[starting_bit_pos/8] & (mask >> (starting_bit_pos % 8));

    //Return first 1 position in masked byte if any + offset
    if(masked_byte != 0)
        return 7 - log2_8(masked_byte) + (starting_bit_pos/8*8);

    
    for(std::size_t i = starting_bit_pos/8+1; i < data8_size; ++i)
    {
        if(data[i] != 0)
            return 7 - log2_8(data8_vec[i]) + i*8;
    }

    throw std::runtime_error("BitPacker::get_next_one_pos : Couldn't find any more one from this position");
}

void BitPacker::print() const
{
    const std::uint8_t* const data8_vec = reinterpret_cast<const std::uint8_t* const>(data.data());
    const std::size_t data8_size = (bit_position+7)/8;
    for(std::size_t i = 0; i < data8_size; ++i)
    {
        printByte(data8_vec[i]);
        std::cout << ' ';
    }

    std::cout << std::endl;
}

void BitPacker::serialize(const std::string& output_file) const
{
    serialize(output_file, data, bit_position, packed_values);
}

void BitPacker::serialize(const std::string& output_file, const std::vector<std::uint64_t>& data, std::uint64_t bit_position, std::uint64_t packed_values)
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

    //Serialize payload as big endian
    const std::size_t payload_size = bit_position / (sizeof(std::uint64_t)*8);
    std::size_t i = 0;
    for(; i < payload_size; ++i)
    {
        const std::uint64_t vBE = toBigEndian64(data[i]);
        f.write(reinterpret_cast<const char*>(&vBE), sizeof(std::uint64_t));
    }

    //Serialize remaining payload byte
    std::size_t remaining_bits = bit_position % (sizeof(std::uint64_t)*8);
    if(remaining_bits != 0)
    {
        const std::uint64_t vBE = toBigEndian64(data[i]);
        f.write(reinterpret_cast<const char*>(&vBE), (remaining_bits+7)/8);
    }

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

    data.resize((bit_position+sizeof(std::uint64_t)*8-1)/(sizeof(std::uint64_t)*8));

    if(!f.read(reinterpret_cast<char*>(data.data()), (bit_position+7)/8))
    {
        f.close();
        throw std::runtime_error("BitPacker::deserialize : unexpected file size");
    }

    f.close();
}

BitPacker::~BitPacker(){}