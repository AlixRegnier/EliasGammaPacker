#ifndef EGP_ELIAS_GAMMA_PACKER_H
#define EGP_ELIAS_GAMMA_PACKER_H

#include "bitpacker.h"

class EliasGammaPacker : public BitPacker
{
private:
    std::size_t next_elias_position{0};
    std::uint8_t first_bit{0}; //First run of bits

    static std::uint64_t elias_delta_bit_length(std::uint64_t x)
    {
        return 2*log2_64(x)+1;
    }
public:
    EliasGammaPacker() = default;

    void pack(std::uint64_t value)
    {
        BitPacker::pack(value, elias_delta_bit_length(value));
    }

    std::uint64_t unpack()
    {
        std::size_t previous_position = next_elias_position;
        next_elias_position = 2*get_next_one_pos(previous_position)-previous_position+1;
        return BitPacker::unpack(previous_position, next_elias_position-previous_position);
    }

    // Get first bit value for a run (0: 00000...0, 1: 11111....1)
    std::uint8_t get_first_bit() const
    {
        return first_bit;
    }

    // Set first bit value for a run (0: 00000...0, 1: 11111....1)
    void set_first_bit(std::uint8_t bit)
    {
        first_bit = bit & std::uint8_t{1};
    }

    void serialize(const std::string& output_file) const override
    {
        //Serialize number of packed values and starting bit_value
        std::uint64_t packed_values_and_first_bit = packed_values << 1 | first_bit;

        //Serialize
        BitPacker::serialize(output_file, data, file_size, bit_position, packed_values_and_first_bit);
    }

    void deserialize(const std::string& input_file) override
    {
        //Deserialize
        BitPacker::deserialize(input_file);

        //Unpack first_bit value
        first_bit = packed_values & std::uint8_t{1};
        packed_values >>= 1;
    }
};

#endif
