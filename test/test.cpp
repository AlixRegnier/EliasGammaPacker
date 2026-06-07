#include "../bitpacker.h"
#include "../egprle.h"
#include <string>
#include <random>

#define ASSERT(expr, msg) massert((expr), "Assertion failed "#expr",", msg)
#define ASSERT_EQ(a, b, function_name, value) ASSERT((a == b), function_name"(" + std::to_string(value) + "): " + std::to_string(a) + " == " + std::to_string(b))

#define TEST_FUNC(f) { \
        bool b = f(); \
        if(b) std::cout << #f":\tSUCCESS" << std::endl; \
        else  std::cout << #f":\tFAILED"  << std::endl; \
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
        std::uint8_t test = BitPacker::log2_8(value);
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

        std::uint8_t test = BitPacker::log2_64(value);
        std::uint8_t truth = 0;
        
        if(!ASSERT_EQ(truth, test, "log2_64", value))
            return false;
    }

    for (int i = 0; i < 10'000; ++i) {
        std::uint64_t value = dist(rng);

        if(value == 0)
            continue;

        std::uint8_t test = BitPacker::log2_64(value);
        std::uint8_t truth = 63 - __builtin_clzll(std::uint64_t{value});
        
        if(!ASSERT_EQ(truth, test, "log2_64", value))
            return false;
    }

    return true;
}

bool test_set_bits()
{
    std::uint8_t s[4096] = {0};
    EliasGammaPacker::EGPRLE::setBits(s, 0, 24);

    for(int i = 0; i < 200; ++i)
    {
        if(!ASSERT_EQ(truth, test, "log2_64", value))
        {
            return false;   
        }
    }
}

bool test_decode_runs()
{
    
}

int main()
{
    TEST_FUNC(test_log2_8)
    TEST_FUNC(test_log2_64)
}