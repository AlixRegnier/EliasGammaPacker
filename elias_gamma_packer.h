#ifndef EGP_ELIAS_GAMMA_PACKER_H
#define EGP_ELIAS_GAMMA_PACKER_H

#include "bitpacker.h"

class EliasGammaPacker : public BitPacker
{
private:
    std::size_t next_elias_position{0};

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
};

#endif
