#ifndef EGP_BITPACKER_H
#define EGP_BITPACKER_H

#include <cstdint>
#include <stdexcept>
#include <bits/stdc++.h>
#include <vector>

class BitPacker 
{
protected:
    std::vector<std::uint8_t> data;
    std::uint64_t bit_position{0};  // Current bit position in the data vector
    std::uint64_t packed_values{0}; // Number of times "pack()" as been called

    static const std::uint8_t tab64[64];
    static const std::uint8_t tab8[8];

    static std::uint8_t log2_64(std::uint64_t value)
    {
        value |= value >> 1;
        value |= value >> 2;
        value |= value >> 4;
        value |= value >> 8;
        value |= value >> 16;
        value |= value >> 32;
        return tab64[((std::uint64_t)((value - (value >> 1))*0x07EDD5E59A4E28C2)) >> 58];
    }

    static std::uint8_t log2_8(std::uint8_t value)
    {
        value |= value >> 1;
        value |= value >> 2;
        value |= value >> 4;
        return tab8[((std::uint8_t)((value - (value >> 1))*0x1D)) >> 5];
    }

    static void printByte(std::uint8_t byte);
public:
    BitPacker() = default;
    virtual ~BitPacker();

    // Pack a value with specified number of bits
    void pack(std::uint64_t value, std::size_t num_bits);

    // Unpack a value with specified number of bits
    std::uint64_t unpack(std::size_t start_bit, std::size_t num_bits) const;

    // Get position of next bit set to one from a starting position
    std::size_t get_next_one_pos(std::size_t starting_bit_pos);

    void serialize(const std::string& output_file) const;

    void deserialize(const std::string& input_file);
    
    // Get the underlying data
    const std::vector<std::uint8_t>& get_data() const 
    {
        return data;
    }

    // Get the number of bits packed
    std::uint64_t get_bit_count() const
    {
        return bit_position;
    }

    // Get the number of packed values
    std::uint64_t get_packed_values_count() const
    {
        return packed_values;
    }

    // Get the number of bytes used (rounded up)
    std::size_t get_byte_count() const
    {
        return data.size();
    }

    // Clear all data
    void clear()
    {
        data.clear();
        bit_position = 0;
    }

    // Reset to allow repacking from the beginning
    void reset()
    {
        bit_position = 0;
        std::fill(data.begin(), data.end(), 0);
    }

    void print() const;
};

#endif