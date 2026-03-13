#include "bitpacker.h"

#include <cstdint>
#include <stdexcept>
#include <bits/stdc++.h>
#include <vector>

//log2_64(0) = 63
const std::uint8_t BitPacker::tab64[] = {
    63,  0, 58,  1, 59, 47, 53,  2,
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
    std::cout << ' ';
}

// Pack a value with specified number of bits
void BitPacker::pack(std::uint64_t value, std::size_t num_bits) {
    #ifndef EGP_NOCHECK
        if (num_bits == 0 || num_bits > sizeof(std::uint64_t)*8) {
            throw std::invalid_argument("BitPacker::pack : num_bits must be between 1 and 64");
        }

        // Ensure value fits in num_bits
        if (num_bits < sizeof(std::uint64_t)*8 && value >= (std::uint64_t{1} << num_bits)) {
            throw std::invalid_argument("BitPacker::pack : value too large for specified number of bits");
        }
    #endif

    ++packed_values;

    constexpr unsigned log64 = 6;
    constexpr unsigned mod64 = 63;
    constexpr unsigned siz64 = 8*sizeof(std::uint64_t);

    const std::size_t p_index =  bit_position >> log64; //divide by (8*sizeof(std::uint64_t))
    const std::size_t p_offset = bit_position  & mod64; //mod    by (8*sizeof(std::uint64_t))

    #ifndef EGP_NOCHECK
        // Ensure there is enough place, increase size by 50% if not
        std::size_t bytes_needed = (bit_position + num_bits + sizeof(std::uint64_t)*8 - 1) / (8*sizeof(std::uint64_t));
        if (data.size() < bytes_needed)
            data.resize(std::max(data.size()*3/2, bytes_needed));
    #endif

    //Fit in current word
    if(p_offset + num_bits <= siz64)
    {
        const unsigned shift = siz64 - p_offset - num_bits;
        data[p_index] |= value << shift;
    }
    //Need to split
    else
    {
        const unsigned shift = p_offset + num_bits - siz64;

        const std::uint64_t mask = (std::uint64_t{1} << shift) - 1;
        data[p_index] |=  value >> shift;
        data[p_index+1] = (value & mask) << (siz64 - shift);
    }

    bit_position += num_bits;
}

// Unpack a value with specified number of bits
std::uint64_t BitPacker::unpack(std::size_t start_bit, std::size_t num_bits) const {
    #ifndef EGP_NOCHECK
        if (num_bits == 0 || num_bits > sizeof(std::uint64_t)*8) {
            throw std::invalid_argument("BitPacker::unpack : num_bits must be between 1 and 64");
        }

        if (start_bit + num_bits > bit_position) {
            throw std::out_of_range("BitPacker::unpack : attempting to read beyond packed data");
        }
    #endif

    constexpr unsigned log64 = 6;
    constexpr unsigned mod64 = 63;
    constexpr unsigned siz64 = 8*sizeof(std::uint64_t);

    const std::size_t p_index =  start_bit >> log64; //divide by (8*sizeof(std::uint64_t))
    const std::size_t p_offset = start_bit  & mod64; //mod    by (8*sizeof(std::uint64_t))
    
    const std::uint64_t left = toLittleEndian64(data[p_index]);

    //Fit in current word
    if(p_offset + num_bits <= siz64)
    {
        const unsigned shift = siz64 - num_bits - p_offset;
        const std::uint64_t mask = (std::uint64_t{1} << num_bits) - 1;
        return (left >> shift) & mask;
    }
    //Need to split
    else
    {
        const std::uint64_t right = toLittleEndian64(data[p_index+1]);

        const unsigned shift = p_offset + num_bits - siz64;
        const std::uint64_t mask = (std::uint64_t{1} << (num_bits-shift)) - 1;
        
        return ((left & mask) << shift) | (right >> (siz64 - shift));
    }
}

std::size_t BitPacker::get_next_one_pos(std::size_t starting_bit_pos) const
{
    constexpr std::size_t siz64 = sizeof(std::uint64_t)*8;

    const std::size_t payload_size = (bit_position + siz64 - 1) / siz64;

    const std::uint64_t mask = ~std::uint64_t{0};

    //Mask bits before bit starting position
    const std::uint64_t masked_u64 = data[starting_bit_pos/siz64] & (mask >> (starting_bit_pos % siz64));

    //Return first 1 position in masked byte if any + offset
    if(masked_u64 != 0)
        return siz64 - 1 - log2_64(masked_u64) + (starting_bit_pos/siz64*siz64);
    
    for(std::size_t i = starting_bit_pos/siz64+1; i < payload_size; ++i)
    {
        if(data[i] != 0)
            return siz64 - 1 - log2_64(data[i]) + i*siz64;
    }

    throw std::runtime_error("BitPacker::get_next_one_pos : Couldn't find any more one from this position");
}

void BitPacker::print() const
{
    const std::uint8_t* const data8_vec = reinterpret_cast<const std::uint8_t* const>(data.data());
    const std::size_t data8_size = (bit_position+7)/8;
    std::size_t i = 0;
    for(; i < data8_size; ++i)
    {
        printByte(data8_vec[7 - (i % 8) + i/8*8]);
        std::cout << ' ';


        if(i % 8 == 7 && i + 1 != data8_size)
            std::cout << std::endl;
    }

    std::cout << "#" << std::endl;
}

void BitPacker::serialize(const std::string& output_file) const
{
    serialize(output_file, data, file_size, bit_position, packed_values);
}

void BitPacker::serialize(const std::string& output_file, const std::vector<std::uint64_t>& data, std::uint64_t file_size, std::uint64_t bit_position, std::uint64_t packed_values)
{
    std::ofstream f(output_file, std::ofstream::binary);

    if(!f.is_open())
    {
        f.close();
        throw std::runtime_error("BitPacker::serialize : couldn't open file '" + output_file + "'");
    }

    //Serialize file size
    f.write(reinterpret_cast<const char*>(&file_size), sizeof(file_size));

    //Serialize bit position
    f.write(reinterpret_cast<const char*>(&bit_position), sizeof(bit_position));

    //Serialize number of packed values
    f.write(reinterpret_cast<const char*>(&packed_values), sizeof(packed_values));

    //Serialize payload as big endian
    const std::size_t payload_size = bit_position / (sizeof(std::uint64_t)*8);
    std::size_t i = 0;
    for(; i < payload_size; ++i)
    {
        const std::uint64_t vBE = toLittleEndian64(data[i]);
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
    
    //Deserialize file size
    f.read(reinterpret_cast<char*>(&file_size), sizeof(file_size));

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

    //Reverse bytes of last integers
    if(bit_position % sizeof(std::uint64_t)*8 != 0)
        data.back() = toBigEndian64(data.back());

    f.close();
}

BitPacker::~BitPacker(){}