#ifndef EGP_UTILS_H
#define EGP_UTILS_H

#include <cstdint>
#include <type_traits>

namespace EliasGammaPacker
{
    static const int nb_runs = 16;
    static const int lane_width = 256;
    static const int lane_nb = 2;
    static const int sublane_width = lane_width * lane_nb / nb_runs;
    static const int selector_width = sublane_width;
    static const int values_offset = nb_runs / 2;

    #define V8_x_32(x) {x, x, x, x, x, x, x, x}

    alignas(32) static constexpr std::uint32_t mask_lsb[33][8] =
    {
        V8_x_32(std::uint32_t{0x0}),
        V8_x_32(std::uint32_t{0x01}),
        V8_x_32(std::uint32_t{0x03}),
        V8_x_32(std::uint32_t{0x07}),
        V8_x_32(std::uint32_t{0x0F}),
        V8_x_32(std::uint32_t{0x1F}),
        V8_x_32(std::uint32_t{0x3F}),
        V8_x_32(std::uint32_t{0x7F}),
        V8_x_32(std::uint32_t{0xFF}),
        V8_x_32(std::uint32_t{0x1FF}),
        V8_x_32(std::uint32_t{0x3FF}),
        V8_x_32(std::uint32_t{0x7FF}),
        V8_x_32(std::uint32_t{0xFFF}),
        V8_x_32(std::uint32_t{0x1FFF}),
        V8_x_32(std::uint32_t{0x3FFF}),
        V8_x_32(std::uint32_t{0x7FFF}),
        V8_x_32(std::uint32_t{0xFFFF}),
        V8_x_32(std::uint32_t{0x1FFFF}),
        V8_x_32(std::uint32_t{0x3FFFF}),
        V8_x_32(std::uint32_t{0x7FFFF}),
        V8_x_32(std::uint32_t{0xFFFFF}),
        V8_x_32(std::uint32_t{0x1FFFFF}),
        V8_x_32(std::uint32_t{0x3FFFFF}),
        V8_x_32(std::uint32_t{0x7FFFFF}),
        V8_x_32(std::uint32_t{0xFFFFFF}),
        V8_x_32(std::uint32_t{0x1FFFFFF}),
        V8_x_32(std::uint32_t{0x3FFFFFF}),
        V8_x_32(std::uint32_t{0x7FFFFFF}),
        V8_x_32(std::uint32_t{0xFFFFFFF}),
        V8_x_32(std::uint32_t{0x1FFFFFFF}),
        V8_x_32(std::uint32_t{0x3FFFFFFF}),
        V8_x_32(std::uint32_t{0x7FFFFFFF}),
        V8_x_32(std::uint32_t{0xFFFFFFFF}),
    };

    alignas(32) static constexpr std::uint32_t mask_lsb_not[33][8] =
    {
        V8_x_32(~std::uint32_t{0x0}),
        V8_x_32(~std::uint32_t{0x01}),
        V8_x_32(~std::uint32_t{0x03}),
        V8_x_32(~std::uint32_t{0x07}),
        V8_x_32(~std::uint32_t{0x0F}),
        V8_x_32(~std::uint32_t{0x1F}),
        V8_x_32(~std::uint32_t{0x3F}),
        V8_x_32(~std::uint32_t{0x7F}),
        V8_x_32(~std::uint32_t{0xFF}),
        V8_x_32(~std::uint32_t{0x1FF}),
        V8_x_32(~std::uint32_t{0x3FF}),
        V8_x_32(~std::uint32_t{0x7FF}),
        V8_x_32(~std::uint32_t{0xFFF}),
        V8_x_32(~std::uint32_t{0x1FFF}),
        V8_x_32(~std::uint32_t{0x3FFF}),
        V8_x_32(~std::uint32_t{0x7FFF}),
        V8_x_32(~std::uint32_t{0xFFFF}),
        V8_x_32(~std::uint32_t{0x1FFFF}),
        V8_x_32(~std::uint32_t{0x3FFFF}),
        V8_x_32(~std::uint32_t{0x7FFFF}),
        V8_x_32(~std::uint32_t{0xFFFFF}),
        V8_x_32(~std::uint32_t{0x1FFFFF}),
        V8_x_32(~std::uint32_t{0x3FFFFF}),
        V8_x_32(~std::uint32_t{0x7FFFFF}),
        V8_x_32(~std::uint32_t{0xFFFFFF}),
        V8_x_32(~std::uint32_t{0x1FFFFFF}),
        V8_x_32(~std::uint32_t{0x3FFFFFF}),
        V8_x_32(~std::uint32_t{0x7FFFFFF}),
        V8_x_32(~std::uint32_t{0xFFFFFFF}),
        V8_x_32(~std::uint32_t{0x1FFFFFFF}),
        V8_x_32(~std::uint32_t{0x3FFFFFFF}),
        V8_x_32(~std::uint32_t{0x7FFFFFFF}),
        V8_x_32(~std::uint32_t{0xFFFFFFFF}),
    };

    static constexpr std::uint8_t bit_run_mask[9] =
    {
        std::uint8_t{0x0},
        std::uint8_t{0x01},
        std::uint8_t{0x03},
        std::uint8_t{0x07},
        std::uint8_t{0x0F},
        std::uint8_t{0x1F},
        std::uint8_t{0x3F},
        std::uint8_t{0x7F},
        std::uint8_t{0xFF}
    };

    #undef V8_x_32

    static constexpr std::uint8_t LUT_log2_8[] = {
   255, 0, 1, 1, 2, 2, 2, 2,
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

    // Swap bytes of a 64-bit integer to big endian
    std::uint64_t inline toBigEndian64(std::uint64_t val) {
        #if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
            return val;  // Already big endian
        #else
            return __builtin_bswap64(val);
        #endif
    }
    
    // Swap bytes of a 64-bit integer to little endian
    std::uint64_t inline toLittleEndian64(std::uint64_t val) {
        #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
            return val;  // Already little endian
        #else
            return __builtin_bswap64(val);
        #endif
    }

    template <typename T>
    int inline log2(T value)
    {
        if constexpr (sizeof(T)*8 <= 8)
            return LUT_log2_8[value];
        else if constexpr (sizeof(T)*8 <= 32)
            return 31 - __builtin_clz(value);
        else
            return 63 - __builtin_clzll(value);
    }
    
    template<typename T>
    T inline unary(T x)
    {
        return T{1} << x;
    }

    //TODO: compile-time fallback if __builtin_ctz is not available
    int inline trailing_zeroes(int x)
    {
        return __builtin_ctz(x);
    }
}

#endif