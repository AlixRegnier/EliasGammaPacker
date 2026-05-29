#ifndef EGP_UTILS_H
#define EGP_UTILS_H

#include <cstdint>
#include <type_traits>

namespace EliasGammaPacker
{
    template <std::uint64_t V>
    using smallest_uint_t =
        std::conditional_t<
            (V <= UINT8_MAX),  std::uint8_t,
        std::conditional_t<
            (V <= UINT16_MAX), std::uint16_t,
        std::conditional_t<
            (V <= UINT32_MAX), std::uint32_t,
                                std::uint64_t>>>;

    template <std::uint64_t V>
    using compile_time_uint_t =
        std::conditional_t<
            (V == 8),  std::uint8_t,
        std::conditional_t<
            (V == 16), std::uint16_t,
        std::conditional_t<
            (V == 32), std::uint32_t,
                         std::uint64_t>>>;

    #define V8_x_32(x) {x, x, x, x, x, x, x, x}

    alignas(32) static const std::uint32_t mask_lsb[33][8]=
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

    alignas(32) static const std::uint32_t mask_lsb_not[33][8] =
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

    #undef V8_x_32

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
    
    // Floor 
    std::uint8_t inline log2_64(std::uint64_t value)
    {
        // value |= value >> 1;
        // value |= value >> 2;
        // value |= value >> 4;
        // value |= value >> 8;
        // value |= value >> 16;
        // value |= value >> 32;
        // return tab64[((std::uint64_t)((value - (value >> 1))*0x07EDD5E59A4E28C2)) >> 58];
        return 63 - __builtin_clzll(value);
    }
    
    template<typename T>
    T inline unary(T x)
    {
        return T{1} << x;
    }
}

#endif