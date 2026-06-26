#include "egprle.h"

//Computed goto if on compiler supporting label addressing through variables
#if !defined(NO_COMPUTED_GOTO) && (defined(__GNUC__) || defined(__clang__))
    #define COMPUTED_GOTO
#endif

#ifdef COMPUTED_GOTO
    #define TARGET(state, x) transition_##state##_##x:
#else
    #define TARGET(state, x) case (x << 1 | state):
#endif

#define GOTO_LOOP continue;

//Apply to each push
//As a run-length of 0 can't exist
//Save bits by substracting 1 to each run-length
#define MOD_PUSH(x) (x-1)

namespace EliasGammaPacker
{

void EGPRLE::BitRunDFA(const std::uint8_t* const input, std::size_t input_size)
{
    bit_run_dfa_struct_t& d = dfa_data;

    #ifdef COMPUTED_GOTO
    const void* transitions[2][256] = {
        {&&transition_0_0,&&transition_0_1,&&transition_0_2,&&transition_0_3,&&transition_0_4,&&transition_0_5,&&transition_0_6,&&transition_0_7,&&transition_0_8,&&transition_0_9,&&transition_0_10,&&transition_0_11,&&transition_0_12,&&transition_0_13,&&transition_0_14,&&transition_0_15,&&transition_0_16,&&transition_0_17,&&transition_0_18,&&transition_0_19,&&transition_0_20,&&transition_0_21,&&transition_0_22,&&transition_0_23,&&transition_0_24,&&transition_0_25,&&transition_0_26,&&transition_0_27,&&transition_0_28,&&transition_0_29,&&transition_0_30,&&transition_0_31,&&transition_0_32,&&transition_0_33,&&transition_0_34,&&transition_0_35,&&transition_0_36,&&transition_0_37,&&transition_0_38,&&transition_0_39,&&transition_0_40,&&transition_0_41,&&transition_0_42,&&transition_0_43,&&transition_0_44,&&transition_0_45,&&transition_0_46,&&transition_0_47,&&transition_0_48,&&transition_0_49,&&transition_0_50,&&transition_0_51,&&transition_0_52,&&transition_0_53,&&transition_0_54,&&transition_0_55,&&transition_0_56,&&transition_0_57,&&transition_0_58,&&transition_0_59,&&transition_0_60,&&transition_0_61,&&transition_0_62,&&transition_0_63,&&transition_0_64,&&transition_0_65,&&transition_0_66,&&transition_0_67,&&transition_0_68,&&transition_0_69,&&transition_0_70,&&transition_0_71,&&transition_0_72,&&transition_0_73,&&transition_0_74,&&transition_0_75,&&transition_0_76,&&transition_0_77,&&transition_0_78,&&transition_0_79,&&transition_0_80,&&transition_0_81,&&transition_0_82,&&transition_0_83,&&transition_0_84,&&transition_0_85,&&transition_0_86,&&transition_0_87,&&transition_0_88,&&transition_0_89,&&transition_0_90,&&transition_0_91,&&transition_0_92,&&transition_0_93,&&transition_0_94,&&transition_0_95,&&transition_0_96,&&transition_0_97,&&transition_0_98,&&transition_0_99,&&transition_0_100,&&transition_0_101,&&transition_0_102,&&transition_0_103,&&transition_0_104,&&transition_0_105,&&transition_0_106,&&transition_0_107,&&transition_0_108,&&transition_0_109,&&transition_0_110,&&transition_0_111,&&transition_0_112,&&transition_0_113,&&transition_0_114,&&transition_0_115,&&transition_0_116,&&transition_0_117,&&transition_0_118,&&transition_0_119,&&transition_0_120,&&transition_0_121,&&transition_0_122,&&transition_0_123,&&transition_0_124,&&transition_0_125,&&transition_0_126,&&transition_0_127,&&transition_0_128,&&transition_0_129,&&transition_0_130,&&transition_0_131,&&transition_0_132,&&transition_0_133,&&transition_0_134,&&transition_0_135,&&transition_0_136,&&transition_0_137,&&transition_0_138,&&transition_0_139,&&transition_0_140,&&transition_0_141,&&transition_0_142,&&transition_0_143,&&transition_0_144,&&transition_0_145,&&transition_0_146,&&transition_0_147,&&transition_0_148,&&transition_0_149,&&transition_0_150,&&transition_0_151,&&transition_0_152,&&transition_0_153,&&transition_0_154,&&transition_0_155,&&transition_0_156,&&transition_0_157,&&transition_0_158,&&transition_0_159,&&transition_0_160,&&transition_0_161,&&transition_0_162,&&transition_0_163,&&transition_0_164,&&transition_0_165,&&transition_0_166,&&transition_0_167,&&transition_0_168,&&transition_0_169,&&transition_0_170,&&transition_0_171,&&transition_0_172,&&transition_0_173,&&transition_0_174,&&transition_0_175,&&transition_0_176,&&transition_0_177,&&transition_0_178,&&transition_0_179,&&transition_0_180,&&transition_0_181,&&transition_0_182,&&transition_0_183,&&transition_0_184,&&transition_0_185,&&transition_0_186,&&transition_0_187,&&transition_0_188,&&transition_0_189,&&transition_0_190,&&transition_0_191,&&transition_0_192,&&transition_0_193,&&transition_0_194,&&transition_0_195,&&transition_0_196,&&transition_0_197,&&transition_0_198,&&transition_0_199,&&transition_0_200,&&transition_0_201,&&transition_0_202,&&transition_0_203,&&transition_0_204,&&transition_0_205,&&transition_0_206,&&transition_0_207,&&transition_0_208,&&transition_0_209,&&transition_0_210,&&transition_0_211,&&transition_0_212,&&transition_0_213,&&transition_0_214,&&transition_0_215,&&transition_0_216,&&transition_0_217,&&transition_0_218,&&transition_0_219,&&transition_0_220,&&transition_0_221,&&transition_0_222,&&transition_0_223,&&transition_0_224,&&transition_0_225,&&transition_0_226,&&transition_0_227,&&transition_0_228,&&transition_0_229,&&transition_0_230,&&transition_0_231,&&transition_0_232,&&transition_0_233,&&transition_0_234,&&transition_0_235,&&transition_0_236,&&transition_0_237,&&transition_0_238,&&transition_0_239,&&transition_0_240,&&transition_0_241,&&transition_0_242,&&transition_0_243,&&transition_0_244,&&transition_0_245,&&transition_0_246,&&transition_0_247,&&transition_0_248,&&transition_0_249,&&transition_0_250,&&transition_0_251,&&transition_0_252,&&transition_0_253,&&transition_0_254,&&transition_0_255},
        {&&transition_1_0,&&transition_1_1,&&transition_1_2,&&transition_1_3,&&transition_1_4,&&transition_1_5,&&transition_1_6,&&transition_1_7,&&transition_1_8,&&transition_1_9,&&transition_1_10,&&transition_1_11,&&transition_1_12,&&transition_1_13,&&transition_1_14,&&transition_1_15,&&transition_1_16,&&transition_1_17,&&transition_1_18,&&transition_1_19,&&transition_1_20,&&transition_1_21,&&transition_1_22,&&transition_1_23,&&transition_1_24,&&transition_1_25,&&transition_1_26,&&transition_1_27,&&transition_1_28,&&transition_1_29,&&transition_1_30,&&transition_1_31,&&transition_1_32,&&transition_1_33,&&transition_1_34,&&transition_1_35,&&transition_1_36,&&transition_1_37,&&transition_1_38,&&transition_1_39,&&transition_1_40,&&transition_1_41,&&transition_1_42,&&transition_1_43,&&transition_1_44,&&transition_1_45,&&transition_1_46,&&transition_1_47,&&transition_1_48,&&transition_1_49,&&transition_1_50,&&transition_1_51,&&transition_1_52,&&transition_1_53,&&transition_1_54,&&transition_1_55,&&transition_1_56,&&transition_1_57,&&transition_1_58,&&transition_1_59,&&transition_1_60,&&transition_1_61,&&transition_1_62,&&transition_1_63,&&transition_1_64,&&transition_1_65,&&transition_1_66,&&transition_1_67,&&transition_1_68,&&transition_1_69,&&transition_1_70,&&transition_1_71,&&transition_1_72,&&transition_1_73,&&transition_1_74,&&transition_1_75,&&transition_1_76,&&transition_1_77,&&transition_1_78,&&transition_1_79,&&transition_1_80,&&transition_1_81,&&transition_1_82,&&transition_1_83,&&transition_1_84,&&transition_1_85,&&transition_1_86,&&transition_1_87,&&transition_1_88,&&transition_1_89,&&transition_1_90,&&transition_1_91,&&transition_1_92,&&transition_1_93,&&transition_1_94,&&transition_1_95,&&transition_1_96,&&transition_1_97,&&transition_1_98,&&transition_1_99,&&transition_1_100,&&transition_1_101,&&transition_1_102,&&transition_1_103,&&transition_1_104,&&transition_1_105,&&transition_1_106,&&transition_1_107,&&transition_1_108,&&transition_1_109,&&transition_1_110,&&transition_1_111,&&transition_1_112,&&transition_1_113,&&transition_1_114,&&transition_1_115,&&transition_1_116,&&transition_1_117,&&transition_1_118,&&transition_1_119,&&transition_1_120,&&transition_1_121,&&transition_1_122,&&transition_1_123,&&transition_1_124,&&transition_1_125,&&transition_1_126,&&transition_1_127,&&transition_1_128,&&transition_1_129,&&transition_1_130,&&transition_1_131,&&transition_1_132,&&transition_1_133,&&transition_1_134,&&transition_1_135,&&transition_1_136,&&transition_1_137,&&transition_1_138,&&transition_1_139,&&transition_1_140,&&transition_1_141,&&transition_1_142,&&transition_1_143,&&transition_1_144,&&transition_1_145,&&transition_1_146,&&transition_1_147,&&transition_1_148,&&transition_1_149,&&transition_1_150,&&transition_1_151,&&transition_1_152,&&transition_1_153,&&transition_1_154,&&transition_1_155,&&transition_1_156,&&transition_1_157,&&transition_1_158,&&transition_1_159,&&transition_1_160,&&transition_1_161,&&transition_1_162,&&transition_1_163,&&transition_1_164,&&transition_1_165,&&transition_1_166,&&transition_1_167,&&transition_1_168,&&transition_1_169,&&transition_1_170,&&transition_1_171,&&transition_1_172,&&transition_1_173,&&transition_1_174,&&transition_1_175,&&transition_1_176,&&transition_1_177,&&transition_1_178,&&transition_1_179,&&transition_1_180,&&transition_1_181,&&transition_1_182,&&transition_1_183,&&transition_1_184,&&transition_1_185,&&transition_1_186,&&transition_1_187,&&transition_1_188,&&transition_1_189,&&transition_1_190,&&transition_1_191,&&transition_1_192,&&transition_1_193,&&transition_1_194,&&transition_1_195,&&transition_1_196,&&transition_1_197,&&transition_1_198,&&transition_1_199,&&transition_1_200,&&transition_1_201,&&transition_1_202,&&transition_1_203,&&transition_1_204,&&transition_1_205,&&transition_1_206,&&transition_1_207,&&transition_1_208,&&transition_1_209,&&transition_1_210,&&transition_1_211,&&transition_1_212,&&transition_1_213,&&transition_1_214,&&transition_1_215,&&transition_1_216,&&transition_1_217,&&transition_1_218,&&transition_1_219,&&transition_1_220,&&transition_1_221,&&transition_1_222,&&transition_1_223,&&transition_1_224,&&transition_1_225,&&transition_1_226,&&transition_1_227,&&transition_1_228,&&transition_1_229,&&transition_1_230,&&transition_1_231,&&transition_1_232,&&transition_1_233,&&transition_1_234,&&transition_1_235,&&transition_1_236,&&transition_1_237,&&transition_1_238,&&transition_1_239,&&transition_1_240,&&transition_1_241,&&transition_1_242,&&transition_1_243,&&transition_1_244,&&transition_1_245,&&transition_1_246,&&transition_1_247,&&transition_1_248,&&transition_1_249,&&transition_1_250,&&transition_1_251,&&transition_1_252,&&transition_1_253,&&transition_1_254,&&transition_1_255}
    };
    #endif

    //Loop sequentially on each input bytes
    while(d.pos < input_size)
    {
        //Return here when enough run lengths were pushed
        if(d.remainder >= nb_runs)
            return;

        std::uint8_t byte = input[d.pos++];

    #ifdef COMPUTED_GOTO
        goto *transitions[d.dfa_state][byte];
    #else
        switch((std::uint16_t{byte} << 1) | d.state)
        {
    #endif

    TARGET(0, 0)
        d.run_length += 8;
        d.state = std::uint8_t{0};
        GOTO_LOOP;
    TARGET(1, 0)
        buffer.push(MOD_PUSH(d.run_length));
        d.run_length = 8;
        d.state = std::uint8_t{0};
        d.remainder += 1;
        GOTO_LOOP;
    TARGET(0, 1)
        buffer.push(MOD_PUSH(d.run_length + 7));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 1;
        GOTO_LOOP;
    TARGET(1, 1)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(7));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(0, 2)
        buffer.push(MOD_PUSH(d.run_length + 6));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(1, 2)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(6));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 3)
        buffer.push(MOD_PUSH(d.run_length + 6));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 1;
        GOTO_LOOP;
    TARGET(1, 3)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(6));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(0, 4)
        buffer.push(MOD_PUSH(d.run_length + 5));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(1, 4)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(5));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 5)
        buffer.push(MOD_PUSH(d.run_length + 5));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 5)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(5));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 6)
        buffer.push(MOD_PUSH(d.run_length + 5));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(1, 6)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(5));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 7)
        buffer.push(MOD_PUSH(d.run_length + 5));
        d.run_length = 3;
        d.state = std::uint8_t{1};
        d.remainder += 1;
        GOTO_LOOP;
    TARGET(1, 7)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(5));
        d.run_length = 3;
        d.state = std::uint8_t{1};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(0, 8)
        buffer.push(MOD_PUSH(d.run_length + 4));
        buffer.push(MOD_PUSH(1));
        d.run_length = 3;
        d.state = std::uint8_t{0};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(1, 8)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(4));
        buffer.push(MOD_PUSH(1));
        d.run_length = 3;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 9)
        buffer.push(MOD_PUSH(d.run_length + 4));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 9)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(4));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 10)
        buffer.push(MOD_PUSH(d.run_length + 4));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 10)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(4));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 11)
        buffer.push(MOD_PUSH(d.run_length + 4));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 11)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(4));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 12)
        buffer.push(MOD_PUSH(d.run_length + 4));
        buffer.push(MOD_PUSH(2));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(1, 12)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(4));
        buffer.push(MOD_PUSH(2));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 13)
        buffer.push(MOD_PUSH(d.run_length + 4));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 13)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(4));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 14)
        buffer.push(MOD_PUSH(d.run_length + 4));
        buffer.push(MOD_PUSH(3));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(1, 14)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(4));
        buffer.push(MOD_PUSH(3));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 15)
        buffer.push(MOD_PUSH(d.run_length + 4));
        d.run_length = 4;
        d.state = std::uint8_t{1};
        d.remainder += 1;
        GOTO_LOOP;
    TARGET(1, 15)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(4));
        d.run_length = 4;
        d.state = std::uint8_t{1};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(0, 16)
        buffer.push(MOD_PUSH(d.run_length + 3));
        buffer.push(MOD_PUSH(1));
        d.run_length = 4;
        d.state = std::uint8_t{0};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(1, 16)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        d.run_length = 4;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 17)
        buffer.push(MOD_PUSH(d.run_length + 3));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 17)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 18)
        buffer.push(MOD_PUSH(d.run_length + 3));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 18)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 19)
        buffer.push(MOD_PUSH(d.run_length + 3));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 19)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 20)
        buffer.push(MOD_PUSH(d.run_length + 3));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 20)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 21)
        buffer.push(MOD_PUSH(d.run_length + 3));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 21)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(0, 22)
        buffer.push(MOD_PUSH(d.run_length + 3));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 22)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 23)
        buffer.push(MOD_PUSH(d.run_length + 3));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 3;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 23)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 3;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 24)
        buffer.push(MOD_PUSH(d.run_length + 3));
        buffer.push(MOD_PUSH(2));
        d.run_length = 3;
        d.state = std::uint8_t{0};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(1, 24)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(2));
        d.run_length = 3;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 25)
        buffer.push(MOD_PUSH(d.run_length + 3));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 25)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 26)
        buffer.push(MOD_PUSH(d.run_length + 3));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 26)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 27)
        buffer.push(MOD_PUSH(d.run_length + 3));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 27)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 28)
        buffer.push(MOD_PUSH(d.run_length + 3));
        buffer.push(MOD_PUSH(3));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(1, 28)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(3));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 29)
        buffer.push(MOD_PUSH(d.run_length + 3));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 29)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 30)
        buffer.push(MOD_PUSH(d.run_length + 3));
        buffer.push(MOD_PUSH(4));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(1, 30)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(4));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 31)
        buffer.push(MOD_PUSH(d.run_length + 3));
        d.run_length = 5;
        d.state = std::uint8_t{1};
        d.remainder += 1;
        GOTO_LOOP;
    TARGET(1, 31)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(3));
        d.run_length = 5;
        d.state = std::uint8_t{1};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(0, 32)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 5;
        d.state = std::uint8_t{0};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(1, 32)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 5;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 33)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(4));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 33)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(4));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 34)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 34)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 35)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 35)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 36)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 36)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 37)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 37)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(0, 38)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 38)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 39)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 3;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 39)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 3;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 40)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 3;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 40)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 3;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 41)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 41)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(0, 42)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(1, 42)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 7;
        GOTO_LOOP;
    TARGET(0, 43)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 43)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(0, 44)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 44)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 45)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 45)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(0, 46)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 46)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 47)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 4;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 47)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 4;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 48)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(2));
        d.run_length = 4;
        d.state = std::uint8_t{0};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(1, 48)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        d.run_length = 4;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 49)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(3));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 49)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(3));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 50)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 50)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 51)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 51)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 52)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 52)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 53)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 53)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(0, 54)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 54)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 55)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 3;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 55)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 3;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 56)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(3));
        d.run_length = 3;
        d.state = std::uint8_t{0};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(1, 56)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(3));
        d.run_length = 3;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 57)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 57)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 58)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 58)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 59)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 59)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 60)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(4));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(1, 60)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(4));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 61)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(4));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 61)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(4));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 62)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(5));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(1, 62)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(5));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 63)
        buffer.push(MOD_PUSH(d.run_length + 2));
        d.run_length = 6;
        d.state = std::uint8_t{1};
        d.remainder += 1;
        GOTO_LOOP;
    TARGET(1, 63)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        d.run_length = 6;
        d.state = std::uint8_t{1};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(0, 64)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 6;
        d.state = std::uint8_t{0};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(1, 64)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 6;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 65)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(5));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 65)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(5));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 66)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(4));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 66)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(4));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 67)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(4));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 67)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(4));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 68)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 68)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 69)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 69)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(0, 70)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 70)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 71)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        d.run_length = 3;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 71)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        d.run_length = 3;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 72)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 3;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 72)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 3;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 73)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 73)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(0, 74)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(1, 74)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 7;
        GOTO_LOOP;
    TARGET(0, 75)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 75)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(0, 76)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 76)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 77)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 77)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(0, 78)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(3));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 78)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(3));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 79)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 4;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 79)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 4;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 80)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 4;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 80)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 4;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 81)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 81)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(0, 82)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(1, 82)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 7;
        GOTO_LOOP;
    TARGET(0, 83)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 83)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(0, 84)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(1, 84)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 7;
        GOTO_LOOP;
    TARGET(0, 85)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 7;
        GOTO_LOOP;
    TARGET(1, 85)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 8;
        GOTO_LOOP;
    TARGET(0, 86)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(1, 86)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 7;
        GOTO_LOOP;
    TARGET(0, 87)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 3;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 87)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 3;
        d.state = std::uint8_t{1};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(0, 88)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 3;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 88)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 3;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 89)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 89)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(0, 90)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(1, 90)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 7;
        GOTO_LOOP;
    TARGET(0, 91)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 91)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(0, 92)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 92)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 93)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 93)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(0, 94)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(4));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 94)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(4));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 95)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 5;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 95)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 5;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 96)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 5;
        d.state = std::uint8_t{0};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(1, 96)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 5;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 97)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(4));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 97)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(4));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 98)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 98)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 99)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(3));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 99)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(3));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 100)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 100)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 101)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 101)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(0, 102)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 102)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 103)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        d.run_length = 3;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 103)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        d.run_length = 3;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 104)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 3;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 104)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 3;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 105)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 105)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(0, 106)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(1, 106)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 7;
        GOTO_LOOP;
    TARGET(0, 107)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 107)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(0, 108)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 108)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 109)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 109)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(0, 110)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 110)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 111)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 4;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 111)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 4;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 112)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(3));
        d.run_length = 4;
        d.state = std::uint8_t{0};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(1, 112)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        d.run_length = 4;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 113)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(3));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 113)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(3));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 114)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 114)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 115)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(2));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 115)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(2));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 116)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 116)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 117)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 117)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(0, 118)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 118)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 119)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        d.run_length = 3;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 119)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        d.run_length = 3;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 120)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(4));
        d.run_length = 3;
        d.state = std::uint8_t{0};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(1, 120)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(4));
        d.run_length = 3;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 121)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(4));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 121)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(4));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 122)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(4));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 122)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(4));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 123)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(4));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 123)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(4));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 124)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(5));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(1, 124)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(5));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 125)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(5));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 125)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(5));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 126)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(6));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(1, 126)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(6));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 127)
        buffer.push(MOD_PUSH(d.run_length + 1));
        d.run_length = 7;
        d.state = std::uint8_t{1};
        d.remainder += 1;
        GOTO_LOOP;
    TARGET(1, 127)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        d.run_length = 7;
        d.state = std::uint8_t{1};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(0, 128)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        d.run_length = 7;
        d.state = std::uint8_t{0};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(1, 128)
        buffer.push(MOD_PUSH(d.run_length + 1));
        d.run_length = 7;
        d.state = std::uint8_t{0};
        d.remainder += 1;
        GOTO_LOOP;
    TARGET(0, 129)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(6));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 129)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(6));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(0, 130)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(5));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 130)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(5));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 131)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(5));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 131)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(5));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(0, 132)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(4));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 132)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(4));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 133)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(4));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 133)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(4));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 134)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(4));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 134)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(4));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 135)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(4));
        d.run_length = 3;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 135)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(4));
        d.run_length = 3;
        d.state = std::uint8_t{1};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(0, 136)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        d.run_length = 3;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 136)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        d.run_length = 3;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 137)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 137)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 138)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(1, 138)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 139)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 139)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 140)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(2));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 140)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(2));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 141)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 141)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 142)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(3));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 142)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(3));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 143)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        d.run_length = 4;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 143)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(3));
        d.run_length = 4;
        d.state = std::uint8_t{1};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(0, 144)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 4;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 144)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 4;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 145)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 145)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 146)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(1, 146)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 147)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 147)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 148)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(1, 148)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 149)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 7;
        GOTO_LOOP;
    TARGET(1, 149)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(0, 150)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(1, 150)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 151)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 3;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 151)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 3;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 152)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        d.run_length = 3;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 152)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        d.run_length = 3;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 153)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 153)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 154)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(1, 154)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 155)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 155)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 156)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(3));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 156)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(3));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 157)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 157)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 158)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(4));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 158)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(4));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 159)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 5;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 159)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 5;
        d.state = std::uint8_t{1};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(0, 160)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 5;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 160)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 5;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 161)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(4));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 161)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(4));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 162)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(1, 162)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 163)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 163)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 164)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(1, 164)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 165)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 7;
        GOTO_LOOP;
    TARGET(1, 165)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(0, 166)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(1, 166)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 167)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 3;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 167)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 3;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 168)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 3;
        d.state = std::uint8_t{0};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(1, 168)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 3;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 169)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 7;
        GOTO_LOOP;
    TARGET(1, 169)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(0, 170)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 8;
        GOTO_LOOP;
    TARGET(1, 170)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 7;
        GOTO_LOOP;
    TARGET(0, 171)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 7;
        GOTO_LOOP;
    TARGET(1, 171)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(0, 172)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(1, 172)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 173)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 7;
        GOTO_LOOP;
    TARGET(1, 173)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(0, 174)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(1, 174)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 175)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 4;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 175)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 4;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 176)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 4;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 176)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 4;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 177)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(3));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 177)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(3));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 178)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(1, 178)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 179)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 179)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 180)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(1, 180)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 181)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 7;
        GOTO_LOOP;
    TARGET(1, 181)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(0, 182)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(1, 182)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 183)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 3;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 183)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 3;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 184)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        d.run_length = 3;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 184)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        d.run_length = 3;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 185)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 185)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 186)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(1, 186)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 187)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 187)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 188)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(4));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 188)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(4));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 189)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(4));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 189)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(4));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 190)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(5));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 190)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(5));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 191)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 6;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 191)
        buffer.push(MOD_PUSH(d.run_length + 1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 6;
        d.state = std::uint8_t{1};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(0, 192)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        d.run_length = 6;
        d.state = std::uint8_t{0};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(1, 192)
        buffer.push(MOD_PUSH(d.run_length + 2));
        d.run_length = 6;
        d.state = std::uint8_t{0};
        d.remainder += 1;
        GOTO_LOOP;
    TARGET(0, 193)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(5));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 193)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(5));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(0, 194)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(4));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 194)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(4));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 195)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(4));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 195)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(4));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(0, 196)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 196)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 197)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 197)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 198)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 198)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 199)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(3));
        d.run_length = 3;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 199)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(3));
        d.run_length = 3;
        d.state = std::uint8_t{1};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(0, 200)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 3;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 200)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 3;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 201)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 201)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 202)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(1, 202)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 203)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 203)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 204)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 204)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 205)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 205)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 206)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(3));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 206)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(3));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 207)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        d.run_length = 4;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 207)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(2));
        d.run_length = 4;
        d.state = std::uint8_t{1};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(0, 208)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 4;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 208)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 4;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 209)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 209)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 210)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(1, 210)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 211)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 211)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 212)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(1, 212)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 213)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 7;
        GOTO_LOOP;
    TARGET(1, 213)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(0, 214)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(1, 214)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 215)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 3;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 215)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 3;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 216)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 3;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 216)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 3;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 217)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 217)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 218)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(1, 218)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 219)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 219)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 220)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 220)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 221)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 221)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 222)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(4));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 222)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(4));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 223)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 5;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 223)
        buffer.push(MOD_PUSH(d.run_length + 2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 5;
        d.state = std::uint8_t{1};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(0, 224)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(3));
        d.run_length = 5;
        d.state = std::uint8_t{0};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(1, 224)
        buffer.push(MOD_PUSH(d.run_length + 3));
        d.run_length = 5;
        d.state = std::uint8_t{0};
        d.remainder += 1;
        GOTO_LOOP;
    TARGET(0, 225)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(4));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 225)
        buffer.push(MOD_PUSH(d.run_length + 3));
        buffer.push(MOD_PUSH(4));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(0, 226)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 226)
        buffer.push(MOD_PUSH(d.run_length + 3));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 227)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(3));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 227)
        buffer.push(MOD_PUSH(d.run_length + 3));
        buffer.push(MOD_PUSH(3));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(0, 228)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 228)
        buffer.push(MOD_PUSH(d.run_length + 3));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 229)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 229)
        buffer.push(MOD_PUSH(d.run_length + 3));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 230)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 230)
        buffer.push(MOD_PUSH(d.run_length + 3));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 231)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(2));
        d.run_length = 3;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 231)
        buffer.push(MOD_PUSH(d.run_length + 3));
        buffer.push(MOD_PUSH(2));
        d.run_length = 3;
        d.state = std::uint8_t{1};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(0, 232)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 3;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 232)
        buffer.push(MOD_PUSH(d.run_length + 3));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 3;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 233)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 233)
        buffer.push(MOD_PUSH(d.run_length + 3));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 234)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 6;
        GOTO_LOOP;
    TARGET(1, 234)
        buffer.push(MOD_PUSH(d.run_length + 3));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(0, 235)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 235)
        buffer.push(MOD_PUSH(d.run_length + 3));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 236)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 236)
        buffer.push(MOD_PUSH(d.run_length + 3));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 237)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 237)
        buffer.push(MOD_PUSH(d.run_length + 3));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 238)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 238)
        buffer.push(MOD_PUSH(d.run_length + 3));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(3));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 239)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(3));
        buffer.push(MOD_PUSH(1));
        d.run_length = 4;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 239)
        buffer.push(MOD_PUSH(d.run_length + 3));
        buffer.push(MOD_PUSH(1));
        d.run_length = 4;
        d.state = std::uint8_t{1};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(0, 240)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(4));
        d.run_length = 4;
        d.state = std::uint8_t{0};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(1, 240)
        buffer.push(MOD_PUSH(d.run_length + 4));
        d.run_length = 4;
        d.state = std::uint8_t{0};
        d.remainder += 1;
        GOTO_LOOP;
    TARGET(0, 241)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(4));
        buffer.push(MOD_PUSH(3));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 241)
        buffer.push(MOD_PUSH(d.run_length + 4));
        buffer.push(MOD_PUSH(3));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(0, 242)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(4));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 242)
        buffer.push(MOD_PUSH(d.run_length + 4));
        buffer.push(MOD_PUSH(2));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 243)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(4));
        buffer.push(MOD_PUSH(2));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 243)
        buffer.push(MOD_PUSH(d.run_length + 4));
        buffer.push(MOD_PUSH(2));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(0, 244)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(4));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 244)
        buffer.push(MOD_PUSH(d.run_length + 4));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 245)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(4));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 5;
        GOTO_LOOP;
    TARGET(1, 245)
        buffer.push(MOD_PUSH(d.run_length + 4));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(0, 246)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(4));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 246)
        buffer.push(MOD_PUSH(d.run_length + 4));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 247)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(4));
        buffer.push(MOD_PUSH(1));
        d.run_length = 3;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 247)
        buffer.push(MOD_PUSH(d.run_length + 4));
        buffer.push(MOD_PUSH(1));
        d.run_length = 3;
        d.state = std::uint8_t{1};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(0, 248)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(5));
        d.run_length = 3;
        d.state = std::uint8_t{0};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(1, 248)
        buffer.push(MOD_PUSH(d.run_length + 5));
        d.run_length = 3;
        d.state = std::uint8_t{0};
        d.remainder += 1;
        GOTO_LOOP;
    TARGET(0, 249)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(5));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 249)
        buffer.push(MOD_PUSH(d.run_length + 5));
        buffer.push(MOD_PUSH(2));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(0, 250)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(5));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 4;
        GOTO_LOOP;
    TARGET(1, 250)
        buffer.push(MOD_PUSH(d.run_length + 5));
        buffer.push(MOD_PUSH(1));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(0, 251)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(5));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 251)
        buffer.push(MOD_PUSH(d.run_length + 5));
        buffer.push(MOD_PUSH(1));
        d.run_length = 2;
        d.state = std::uint8_t{1};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(0, 252)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(6));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(1, 252)
        buffer.push(MOD_PUSH(d.run_length + 6));
        d.run_length = 2;
        d.state = std::uint8_t{0};
        d.remainder += 1;
        GOTO_LOOP;
    TARGET(0, 253)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(6));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 3;
        GOTO_LOOP;
    TARGET(1, 253)
        buffer.push(MOD_PUSH(d.run_length + 6));
        buffer.push(MOD_PUSH(1));
        d.run_length = 1;
        d.state = std::uint8_t{1};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(0, 254)
        buffer.push(MOD_PUSH(d.run_length));
        buffer.push(MOD_PUSH(7));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 2;
        GOTO_LOOP;
    TARGET(1, 254)
        buffer.push(MOD_PUSH(d.run_length + 7));
        d.run_length = 1;
        d.state = std::uint8_t{0};
        d.remainder += 1;
        GOTO_LOOP;
    TARGET(0, 255)
        buffer.push(MOD_PUSH(d.run_length));
        d.run_length = 8;
        d.state = std::uint8_t{1};
        d.remainder += 1;
        GOTO_LOOP;
    TARGET(1, 255)
        d.run_length += 8;
        d.state = std::uint8_t{1};
        GOTO_LOOP;


        #ifndef COMPUTED_GOTO
        }
        #endif
    }

    //Store last run-length
    if(!d.stop)
    {
        d.stop = true;
        buffer.push(MOD_PUSH(d.run_length));
        d.remainder += 1;
    }
}

}