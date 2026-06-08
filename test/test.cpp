#include "../egprle.h"
#include <string>
#include <random>
#include <iostream>

#define ASSERT(expr, msg) massert((expr), "Assertion failed "#expr",", msg)
#define ASSERT_EQ(a, b, function_name, value) ASSERT((a == b), function_name"(" + std::to_string(value) + "): " + std::to_string(a) + " == " + std::to_string(b))

#define TEST_FUNC(f) { \
        bool b = f(); \
        if(b) std::cout << #f":\tSUCCESS" << std::endl; \
        else  std::cout << #f":\tFAILED"  << std::endl; \
    }



void printByte(std::uint8_t byte, char end_chr = '\n')
{
    for(int i = 0; i < 8; ++i)
    {
        if(byte >> (7-i) & std::uint8_t{1})
            std::cout << '1';
        else
            std::cout << '0';
    }
    std::cout << end_chr;
}
    
bool massert(bool expr, const std::string& expr_str, const std::string& msg)
{
    if(!expr)
        std::cerr << expr_str << " caused by:\n\t" << msg << std::endl;

    return expr;
}

bool test_log2_8()
{
    for(std::uint8_t value = 1; value != 0; ++value)
    {
        std::uint8_t test = EliasGammaPacker::log2_8(value);
        std::uint8_t truth = 63 - __builtin_clzll(value);
        
        if(!ASSERT_EQ(truth, test, "log2_8", value))
            return false;
    }

    return true;
}


bool test_log2_64()
{    
    std::mt19937_64 rng(std::random_device{}());
    std::uniform_int_distribution<std::uint64_t> dist(
        std::numeric_limits<std::uint64_t>::min(),
        std::numeric_limits<std::uint64_t>::max()
    );

    {
        std::uint64_t value = 1;

        std::uint8_t test = EliasGammaPacker::log2_64(value);
        std::uint8_t truth = 0;
        
        if(!ASSERT_EQ(truth, test, "log2_64", value))
            return false;
    }

    for (int i = 0; i < 10'000; ++i) {
        std::uint64_t value = dist(rng);

        if(value == 0)
            continue;

        std::uint8_t test = EliasGammaPacker::log2_64(value);
        std::uint8_t truth = 63 - __builtin_clzll(std::uint64_t{value});
        
        if(!ASSERT_EQ(truth, test, "log2_64", value))
            return false;
    }

    return true;
}

bool test_set_bits()
{
    std::uint8_t test1[10] = {0};
    std::uint8_t test2[10] = {0};

    EliasGammaPacker::EGPRLE::setBits(test1, 0, 24);
    EliasGammaPacker::EGPRLE::setBits(test1, 26, 3);
    EliasGammaPacker::EGPRLE::setBits(test1, 45, 13);
    EliasGammaPacker::EGPRLE::setBits(test1, 67, 2);
    EliasGammaPacker::EGPRLE::setBits(test1, 79, 1);

    EliasGammaPacker::EGPRLE::setBits(test2, 1, 23);
    EliasGammaPacker::EGPRLE::setBits(test2, 26, 3);
    EliasGammaPacker::EGPRLE::setBits(test2, 45, 13);
    EliasGammaPacker::EGPRLE::setBits(test2, 67, 2);
    EliasGammaPacker::EGPRLE::setBits(test2, 79, 1);


    std::uint8_t truth1[10] = {
        0b11111111,
        0b11111111,
        0b11111111,
        0b00111000,
        0b00000000,
        0b00000111,
        0b11111111,
        0b11000000,
        0b00011000,
        0b00000001
    };

    std::uint8_t truth2[10] = {
        0b01111111,
        0b11111111,
        0b11111111,
        0b00111000,
        0b00000000,
        0b00000111,
        0b11111111,
        0b11000000,
        0b00011000,
        0b00000001
    };

    for(int i = 0; i < sizeof(truth1); ++i)
    {
        if(!ASSERT_EQ(truth1[i], test1[i], "byte equal", i))
        {
            return false;   
        }
    }

    for(int i = 0; i < sizeof(truth2); ++i)
    {
        if(!ASSERT_EQ(truth2[i], test2[i], "byte equal", i))
        {
            return false;
        }
    }

    return true;
}

bool test_decode_runs()
{
    std::uint8_t truth1[] = {
        0b11111111,
        0b11111111,
        0b11111111,
        0b00111000,
        0b00000000,
        0b00000111,
        0b11111111,
        0b11000000,
        0b00011000,
        0b00000001
    };

    std::uint8_t truth2[] = {
        0b01111111,
        0b11111111,
        0b11111111,
        0b00111000,
        0b00000000,
        0b00000111,
        0b11111111,
        0b11000000,
        0b00011000,
        0b00000001
    };


    bool first_bit_value1 = (truth1[0] >> 7) & 1;
    bool first_bit_value2 = (truth2[0] >> 7) & 1;

    std::uint32_t values1[] = {
        24, 2, 3, 16, 13, 9, 2, 10, 1
    };

    std::uint32_t values2[] = {
        1, 23, 2, 3, 16, 13, 9, 2, 10, 1
    };

    std::uint8_t test1[sizeof(truth1)] = {0};
    std::uint8_t test2[sizeof(truth2)] = {0};

    std::size_t bit_pos1 = 0;
    std::size_t bit_pos2 = 0;

    EliasGammaPacker::EGPRLE::decode_bit_runs(test1, bit_pos1, sizeof(test1)*8, values1, 9, first_bit_value1);
    EliasGammaPacker::EGPRLE::decode_bit_runs(test2, bit_pos2, sizeof(test2)*8, values2, 10, first_bit_value2);

    for(int i = 0; i < sizeof(truth1); ++i)
    {
        if(!ASSERT_EQ(truth1[i], test1[i], "byte equal", i))
        {
            return false;
        }
    }

    if(!ASSERT_EQ(bit_pos1, sizeof(test1)*8, "bit_pos", bit_pos1))
        return false;

    for(int i = 0; i < sizeof(truth2); ++i)
    {
        if(!ASSERT_EQ(truth2[i], test2[i], "byte equal", i))
        {
            return false;
        }
    }

    if(!ASSERT_EQ(bit_pos2, sizeof(test2)*8, "bit_pos", bit_pos2))
        return false;

    return true;
}

int main()
{
    TEST_FUNC(test_log2_8)
    TEST_FUNC(test_log2_64)
    TEST_FUNC(test_set_bits)
    TEST_FUNC(test_decode_runs)
}