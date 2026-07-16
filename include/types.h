#ifndef EGP_TYPES_H
#define EGP_TYPES_H
#include <cstdint>
#include <immintrin.h>
#include "utils.h"

namespace EliasGammaPacker
{
    template <std::uint64_t V>
    struct __smallest_uint {
        static_assert(V <= 64, "smallest_uint can't exceed 64 bits");

        using type =
            std::conditional_t<
                (V <= UINT8_MAX),  std::uint8_t,
            std::conditional_t<
                (V <= UINT16_MAX), std::uint16_t,
            std::conditional_t<
                (V <= UINT32_MAX), std::uint32_t,
                                   std::uint64_t>>>;
    };

    template <std::uint64_t V>
    using smallest_uint_t = typename __smallest_uint<V>::type;

    template <std::uint64_t V>
    struct __static_uint {
        static_assert(V <= 64, "static_uint can't exceed 64 bits");

        using type =
            std::conditional_t<
                (V <= 8),  std::uint8_t,
            std::conditional_t<
                (V <= 16), std::uint16_t,
            std::conditional_t<
                (V <= 32), std::uint32_t,
                           std::uint64_t>>>;
    };

    template <std::uint64_t V>
    using static_uint_t = typename __static_uint<V>::type;

    using run_length_t = std::uint32_t;

    using metadata_t = std::uint64_t;

    struct metadata_struct_t {
        static_uint_t<sizeof(metadata_t)*8> out_size;
        std::uint8_t is_raw;
        std::uint8_t starting_bit_value;
    };

    using selector_t = static_uint_t<selector_width>; //Must be 32-byte aligned

    enum entry_point_t { init, begin, inner };

    using payload_t = __m256i;
    
    struct decode_partial_struct_t {
        payload_t mask;
        payload_t payload1;
        payload_t payload2;

        selector_t selector;

        int remaining_bits;
        int frame_width;
        std::size_t bit_pos;

        entry_point_t next_entry_point;
    };

    struct bit_run_dfa_struct_t {
        std::size_t pos;
        run_length_t run_length;
        std::uint8_t state;
        bool stop;
    };

}

#endif