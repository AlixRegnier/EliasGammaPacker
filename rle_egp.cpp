#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdint>
#include <string>
#include "elias_gamma_packer.h"

#if !defined(COMPUTED_GOTO) && (defined(__GNUC__) || defined(__clang__))
    #define COMPUTED_GOTO
#endif

#ifdef COMPUTED_GOTO
    #define TARGET(state, x) transition_##state##_##x:
#else
    #define TARGET(state, x) case (x << 1 | state):
#endif

#define GOTO_LOOP continue;
    
void BitRunDFA(EliasGammaPacker& egp, const std::uint8_t* const input, std::size_t length) {
    std::uint64_t run_length = 0;
    std::size_t pos = 0;
    std::uint8_t byte = input[pos];
    std::uint8_t state = (byte >> 7) & 1;
    std::uint64_t packed_values = 0;
    
    #ifdef COMPUTED_GOTO
    const void* transitions[2][256] = {
        {&&transition_0_0,&&transition_0_1,&&transition_0_2,&&transition_0_3,&&transition_0_4,&&transition_0_5,&&transition_0_6,&&transition_0_7,&&transition_0_8,&&transition_0_9,&&transition_0_10,&&transition_0_11,&&transition_0_12,&&transition_0_13,&&transition_0_14,&&transition_0_15,&&transition_0_16,&&transition_0_17,&&transition_0_18,&&transition_0_19,&&transition_0_20,&&transition_0_21,&&transition_0_22,&&transition_0_23,&&transition_0_24,&&transition_0_25,&&transition_0_26,&&transition_0_27,&&transition_0_28,&&transition_0_29,&&transition_0_30,&&transition_0_31,&&transition_0_32,&&transition_0_33,&&transition_0_34,&&transition_0_35,&&transition_0_36,&&transition_0_37,&&transition_0_38,&&transition_0_39,&&transition_0_40,&&transition_0_41,&&transition_0_42,&&transition_0_43,&&transition_0_44,&&transition_0_45,&&transition_0_46,&&transition_0_47,&&transition_0_48,&&transition_0_49,&&transition_0_50,&&transition_0_51,&&transition_0_52,&&transition_0_53,&&transition_0_54,&&transition_0_55,&&transition_0_56,&&transition_0_57,&&transition_0_58,&&transition_0_59,&&transition_0_60,&&transition_0_61,&&transition_0_62,&&transition_0_63,&&transition_0_64,&&transition_0_65,&&transition_0_66,&&transition_0_67,&&transition_0_68,&&transition_0_69,&&transition_0_70,&&transition_0_71,&&transition_0_72,&&transition_0_73,&&transition_0_74,&&transition_0_75,&&transition_0_76,&&transition_0_77,&&transition_0_78,&&transition_0_79,&&transition_0_80,&&transition_0_81,&&transition_0_82,&&transition_0_83,&&transition_0_84,&&transition_0_85,&&transition_0_86,&&transition_0_87,&&transition_0_88,&&transition_0_89,&&transition_0_90,&&transition_0_91,&&transition_0_92,&&transition_0_93,&&transition_0_94,&&transition_0_95,&&transition_0_96,&&transition_0_97,&&transition_0_98,&&transition_0_99,&&transition_0_100,&&transition_0_101,&&transition_0_102,&&transition_0_103,&&transition_0_104,&&transition_0_105,&&transition_0_106,&&transition_0_107,&&transition_0_108,&&transition_0_109,&&transition_0_110,&&transition_0_111,&&transition_0_112,&&transition_0_113,&&transition_0_114,&&transition_0_115,&&transition_0_116,&&transition_0_117,&&transition_0_118,&&transition_0_119,&&transition_0_120,&&transition_0_121,&&transition_0_122,&&transition_0_123,&&transition_0_124,&&transition_0_125,&&transition_0_126,&&transition_0_127,&&transition_0_128,&&transition_0_129,&&transition_0_130,&&transition_0_131,&&transition_0_132,&&transition_0_133,&&transition_0_134,&&transition_0_135,&&transition_0_136,&&transition_0_137,&&transition_0_138,&&transition_0_139,&&transition_0_140,&&transition_0_141,&&transition_0_142,&&transition_0_143,&&transition_0_144,&&transition_0_145,&&transition_0_146,&&transition_0_147,&&transition_0_148,&&transition_0_149,&&transition_0_150,&&transition_0_151,&&transition_0_152,&&transition_0_153,&&transition_0_154,&&transition_0_155,&&transition_0_156,&&transition_0_157,&&transition_0_158,&&transition_0_159,&&transition_0_160,&&transition_0_161,&&transition_0_162,&&transition_0_163,&&transition_0_164,&&transition_0_165,&&transition_0_166,&&transition_0_167,&&transition_0_168,&&transition_0_169,&&transition_0_170,&&transition_0_171,&&transition_0_172,&&transition_0_173,&&transition_0_174,&&transition_0_175,&&transition_0_176,&&transition_0_177,&&transition_0_178,&&transition_0_179,&&transition_0_180,&&transition_0_181,&&transition_0_182,&&transition_0_183,&&transition_0_184,&&transition_0_185,&&transition_0_186,&&transition_0_187,&&transition_0_188,&&transition_0_189,&&transition_0_190,&&transition_0_191,&&transition_0_192,&&transition_0_193,&&transition_0_194,&&transition_0_195,&&transition_0_196,&&transition_0_197,&&transition_0_198,&&transition_0_199,&&transition_0_200,&&transition_0_201,&&transition_0_202,&&transition_0_203,&&transition_0_204,&&transition_0_205,&&transition_0_206,&&transition_0_207,&&transition_0_208,&&transition_0_209,&&transition_0_210,&&transition_0_211,&&transition_0_212,&&transition_0_213,&&transition_0_214,&&transition_0_215,&&transition_0_216,&&transition_0_217,&&transition_0_218,&&transition_0_219,&&transition_0_220,&&transition_0_221,&&transition_0_222,&&transition_0_223,&&transition_0_224,&&transition_0_225,&&transition_0_226,&&transition_0_227,&&transition_0_228,&&transition_0_229,&&transition_0_230,&&transition_0_231,&&transition_0_232,&&transition_0_233,&&transition_0_234,&&transition_0_235,&&transition_0_236,&&transition_0_237,&&transition_0_238,&&transition_0_239,&&transition_0_240,&&transition_0_241,&&transition_0_242,&&transition_0_243,&&transition_0_244,&&transition_0_245,&&transition_0_246,&&transition_0_247,&&transition_0_248,&&transition_0_249,&&transition_0_250,&&transition_0_251,&&transition_0_252,&&transition_0_253,&&transition_0_254,&&transition_0_255},
        {&&transition_1_0,&&transition_1_1,&&transition_1_2,&&transition_1_3,&&transition_1_4,&&transition_1_5,&&transition_1_6,&&transition_1_7,&&transition_1_8,&&transition_1_9,&&transition_1_10,&&transition_1_11,&&transition_1_12,&&transition_1_13,&&transition_1_14,&&transition_1_15,&&transition_1_16,&&transition_1_17,&&transition_1_18,&&transition_1_19,&&transition_1_20,&&transition_1_21,&&transition_1_22,&&transition_1_23,&&transition_1_24,&&transition_1_25,&&transition_1_26,&&transition_1_27,&&transition_1_28,&&transition_1_29,&&transition_1_30,&&transition_1_31,&&transition_1_32,&&transition_1_33,&&transition_1_34,&&transition_1_35,&&transition_1_36,&&transition_1_37,&&transition_1_38,&&transition_1_39,&&transition_1_40,&&transition_1_41,&&transition_1_42,&&transition_1_43,&&transition_1_44,&&transition_1_45,&&transition_1_46,&&transition_1_47,&&transition_1_48,&&transition_1_49,&&transition_1_50,&&transition_1_51,&&transition_1_52,&&transition_1_53,&&transition_1_54,&&transition_1_55,&&transition_1_56,&&transition_1_57,&&transition_1_58,&&transition_1_59,&&transition_1_60,&&transition_1_61,&&transition_1_62,&&transition_1_63,&&transition_1_64,&&transition_1_65,&&transition_1_66,&&transition_1_67,&&transition_1_68,&&transition_1_69,&&transition_1_70,&&transition_1_71,&&transition_1_72,&&transition_1_73,&&transition_1_74,&&transition_1_75,&&transition_1_76,&&transition_1_77,&&transition_1_78,&&transition_1_79,&&transition_1_80,&&transition_1_81,&&transition_1_82,&&transition_1_83,&&transition_1_84,&&transition_1_85,&&transition_1_86,&&transition_1_87,&&transition_1_88,&&transition_1_89,&&transition_1_90,&&transition_1_91,&&transition_1_92,&&transition_1_93,&&transition_1_94,&&transition_1_95,&&transition_1_96,&&transition_1_97,&&transition_1_98,&&transition_1_99,&&transition_1_100,&&transition_1_101,&&transition_1_102,&&transition_1_103,&&transition_1_104,&&transition_1_105,&&transition_1_106,&&transition_1_107,&&transition_1_108,&&transition_1_109,&&transition_1_110,&&transition_1_111,&&transition_1_112,&&transition_1_113,&&transition_1_114,&&transition_1_115,&&transition_1_116,&&transition_1_117,&&transition_1_118,&&transition_1_119,&&transition_1_120,&&transition_1_121,&&transition_1_122,&&transition_1_123,&&transition_1_124,&&transition_1_125,&&transition_1_126,&&transition_1_127,&&transition_1_128,&&transition_1_129,&&transition_1_130,&&transition_1_131,&&transition_1_132,&&transition_1_133,&&transition_1_134,&&transition_1_135,&&transition_1_136,&&transition_1_137,&&transition_1_138,&&transition_1_139,&&transition_1_140,&&transition_1_141,&&transition_1_142,&&transition_1_143,&&transition_1_144,&&transition_1_145,&&transition_1_146,&&transition_1_147,&&transition_1_148,&&transition_1_149,&&transition_1_150,&&transition_1_151,&&transition_1_152,&&transition_1_153,&&transition_1_154,&&transition_1_155,&&transition_1_156,&&transition_1_157,&&transition_1_158,&&transition_1_159,&&transition_1_160,&&transition_1_161,&&transition_1_162,&&transition_1_163,&&transition_1_164,&&transition_1_165,&&transition_1_166,&&transition_1_167,&&transition_1_168,&&transition_1_169,&&transition_1_170,&&transition_1_171,&&transition_1_172,&&transition_1_173,&&transition_1_174,&&transition_1_175,&&transition_1_176,&&transition_1_177,&&transition_1_178,&&transition_1_179,&&transition_1_180,&&transition_1_181,&&transition_1_182,&&transition_1_183,&&transition_1_184,&&transition_1_185,&&transition_1_186,&&transition_1_187,&&transition_1_188,&&transition_1_189,&&transition_1_190,&&transition_1_191,&&transition_1_192,&&transition_1_193,&&transition_1_194,&&transition_1_195,&&transition_1_196,&&transition_1_197,&&transition_1_198,&&transition_1_199,&&transition_1_200,&&transition_1_201,&&transition_1_202,&&transition_1_203,&&transition_1_204,&&transition_1_205,&&transition_1_206,&&transition_1_207,&&transition_1_208,&&transition_1_209,&&transition_1_210,&&transition_1_211,&&transition_1_212,&&transition_1_213,&&transition_1_214,&&transition_1_215,&&transition_1_216,&&transition_1_217,&&transition_1_218,&&transition_1_219,&&transition_1_220,&&transition_1_221,&&transition_1_222,&&transition_1_223,&&transition_1_224,&&transition_1_225,&&transition_1_226,&&transition_1_227,&&transition_1_228,&&transition_1_229,&&transition_1_230,&&transition_1_231,&&transition_1_232,&&transition_1_233,&&transition_1_234,&&transition_1_235,&&transition_1_236,&&transition_1_237,&&transition_1_238,&&transition_1_239,&&transition_1_240,&&transition_1_241,&&transition_1_242,&&transition_1_243,&&transition_1_244,&&transition_1_245,&&transition_1_246,&&transition_1_247,&&transition_1_248,&&transition_1_249,&&transition_1_250,&&transition_1_251,&&transition_1_252,&&transition_1_253,&&transition_1_254,&&transition_1_255}
    };
    #endif

    while(pos < length)
    {
        byte = input[pos++];

    #ifdef COMPUTED_GOTO
        goto *transitions[state][byte];
    #else
        switch((std::uint16_t{byte} << 1) | state)
        {
    #endif

        TARGET(0, 0)
            run_length += 8;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 0)
            egp.pack(run_length);
            packed_values += 1;
            run_length = 8;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 1)
            egp.pack(run_length + 7);
            packed_values += 1;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 1)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b00111}, 5);
            packed_values += 2;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 2)
            egp.pack(run_length + 6);
            egp.pack(std::uint64_t{0b1}, 1);
            packed_values += 2;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 2)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b001101}, 6);
            packed_values += 3;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 3)
            egp.pack(run_length + 6);
            packed_values += 1;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 3)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b00110}, 5);
            packed_values += 2;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 4)
            egp.pack(run_length + 5);
            egp.pack(std::uint64_t{0b1}, 1);
            packed_values += 2;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 4)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b001011}, 6);
            packed_values += 3;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 5)
            egp.pack(run_length + 5);
            egp.pack(std::uint64_t{0b11}, 2);
            packed_values += 3;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 5)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b0010111}, 7);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 6)
            egp.pack(run_length + 5);
            egp.pack(std::uint64_t{0b010}, 3);
            packed_values += 2;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 6)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b00101010}, 8);
            packed_values += 3;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 7)
            egp.pack(run_length + 5);
            packed_values += 1;
            run_length = 3;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 7)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b00101}, 5);
            packed_values += 2;
            run_length = 3;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 8)
            egp.pack(run_length + 4);
            egp.pack(std::uint64_t{0b1}, 1);
            packed_values += 2;
            run_length = 3;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 8)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b001001}, 6);
            packed_values += 3;
            run_length = 3;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 9)
            egp.pack(run_length + 4);
            egp.pack(std::uint64_t{0b1010}, 4);
            packed_values += 3;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 9)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b001001010}, 9);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 10)
            egp.pack(run_length + 4);
            egp.pack(std::uint64_t{0b111}, 3);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 10)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b00100111}, 8);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 11)
            egp.pack(run_length + 4);
            egp.pack(std::uint64_t{0b11}, 2);
            packed_values += 3;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 11)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b0010011}, 7);
            packed_values += 4;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 12)
            egp.pack(run_length + 4);
            egp.pack(std::uint64_t{0b010}, 3);
            packed_values += 2;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 12)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b00100010}, 8);
            packed_values += 3;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 13)
            egp.pack(run_length + 4);
            egp.pack(std::uint64_t{0b0101}, 4);
            packed_values += 3;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 13)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b001000101}, 9);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 14)
            egp.pack(run_length + 4);
            egp.pack(std::uint64_t{0b011}, 3);
            packed_values += 2;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 14)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b00100011}, 8);
            packed_values += 3;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 15)
            egp.pack(run_length + 4);
            packed_values += 1;
            run_length = 4;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 15)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b00100}, 5);
            packed_values += 2;
            run_length = 4;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 16)
            egp.pack(run_length + 3);
            egp.pack(std::uint64_t{0b1}, 1);
            packed_values += 2;
            run_length = 4;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 16)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b0111}, 4);
            packed_values += 3;
            run_length = 4;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 17)
            egp.pack(run_length + 3);
            egp.pack(std::uint64_t{0b1011}, 4);
            packed_values += 3;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 17)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b0111011}, 7);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 18)
            egp.pack(run_length + 3);
            egp.pack(std::uint64_t{0b10101}, 5);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 18)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b01110101}, 8);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 19)
            egp.pack(run_length + 3);
            egp.pack(std::uint64_t{0b1010}, 4);
            packed_values += 3;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 19)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b0111010}, 7);
            packed_values += 4;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 20)
            egp.pack(run_length + 3);
            egp.pack(std::uint64_t{0b111}, 3);
            packed_values += 4;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 20)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b011111}, 6);
            packed_values += 5;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 21)
            egp.pack(run_length + 3);
            egp.pack(std::uint64_t{0b1111}, 4);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 21)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b0111111}, 7);
            packed_values += 6;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 22)
            egp.pack(run_length + 3);
            egp.pack(std::uint64_t{0b11010}, 5);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 22)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b01111010}, 8);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 23)
            egp.pack(run_length + 3);
            egp.pack(std::uint64_t{0b11}, 2);
            packed_values += 3;
            run_length = 3;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 23)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b01111}, 5);
            packed_values += 4;
            run_length = 3;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 24)
            egp.pack(run_length + 3);
            egp.pack(std::uint64_t{0b010}, 3);
            packed_values += 2;
            run_length = 3;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 24)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b011010}, 6);
            packed_values += 3;
            run_length = 3;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 25)
            egp.pack(run_length + 3);
            egp.pack(std::uint64_t{0b010010}, 6);
            packed_values += 3;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 25)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b011010010}, 9);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 26)
            egp.pack(run_length + 3);
            egp.pack(std::uint64_t{0b01011}, 5);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 26)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b01101011}, 8);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 27)
            egp.pack(run_length + 3);
            egp.pack(std::uint64_t{0b0101}, 4);
            packed_values += 3;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 27)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b0110101}, 7);
            packed_values += 4;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 28)
            egp.pack(run_length + 3);
            egp.pack(std::uint64_t{0b011}, 3);
            packed_values += 2;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 28)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b011011}, 6);
            packed_values += 3;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 29)
            egp.pack(run_length + 3);
            egp.pack(std::uint64_t{0b0111}, 4);
            packed_values += 3;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 29)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b0110111}, 7);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 30)
            egp.pack(run_length + 3);
            egp.pack(std::uint64_t{0b00100}, 5);
            packed_values += 2;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 30)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b01100100}, 8);
            packed_values += 3;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 31)
            egp.pack(run_length + 3);
            packed_values += 1;
            run_length = 5;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 31)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b011}, 3);
            packed_values += 2;
            run_length = 5;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 32)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b1}, 1);
            packed_values += 2;
            run_length = 5;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 32)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b0101}, 4);
            packed_values += 3;
            run_length = 5;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 33)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b100100}, 6);
            packed_values += 3;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 33)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b010100100}, 9);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 34)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b10111}, 5);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 34)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b01010111}, 8);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 35)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b1011}, 4);
            packed_values += 3;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 35)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b0101011}, 7);
            packed_values += 4;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 36)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b10101}, 5);
            packed_values += 4;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 36)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b01010101}, 8);
            packed_values += 5;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 37)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b101011}, 6);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 37)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b010101011}, 9);
            packed_values += 6;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 38)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b1010010}, 7);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 38)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b0101010010}, 10);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 39)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b1010}, 4);
            packed_values += 3;
            run_length = 3;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 39)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b0101010}, 7);
            packed_values += 4;
            run_length = 3;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 40)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b111}, 3);
            packed_values += 4;
            run_length = 3;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 40)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b010111}, 6);
            packed_values += 5;
            run_length = 3;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 41)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b111010}, 6);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 41)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b010111010}, 9);
            packed_values += 6;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 42)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b11111}, 5);
            packed_values += 6;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 42)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b01011111}, 8);
            packed_values += 7;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 43)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b1111}, 4);
            packed_values += 5;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 43)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b0101111}, 7);
            packed_values += 6;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 44)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b11010}, 5);
            packed_values += 4;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 44)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b01011010}, 8);
            packed_values += 5;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 45)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b110101}, 6);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 45)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b010110101}, 9);
            packed_values += 6;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 46)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b11011}, 5);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 46)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b01011011}, 8);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 47)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b11}, 2);
            packed_values += 3;
            run_length = 4;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 47)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b01011}, 5);
            packed_values += 4;
            run_length = 4;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 48)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b010}, 3);
            packed_values += 2;
            run_length = 4;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 48)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b010010}, 6);
            packed_values += 3;
            run_length = 4;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 49)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b010011}, 6);
            packed_values += 3;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 49)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b010010011}, 9);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 50)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b0100101}, 7);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 50)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b0100100101}, 10);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 51)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b010010}, 6);
            packed_values += 3;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 51)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b010010010}, 9);
            packed_values += 4;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 52)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b01011}, 5);
            packed_values += 4;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 52)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b01001011}, 8);
            packed_values += 5;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 53)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b010111}, 6);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 53)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b010010111}, 9);
            packed_values += 6;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 54)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b0101010}, 7);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 54)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b0100101010}, 10);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 55)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b0101}, 4);
            packed_values += 3;
            run_length = 3;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 55)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b0100101}, 7);
            packed_values += 4;
            run_length = 3;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 56)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b011}, 3);
            packed_values += 2;
            run_length = 3;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 56)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b010011}, 6);
            packed_values += 3;
            run_length = 3;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 57)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b011010}, 6);
            packed_values += 3;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 57)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b010011010}, 9);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 58)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b01111}, 5);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 58)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b01001111}, 8);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 59)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b0111}, 4);
            packed_values += 3;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 59)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b0100111}, 7);
            packed_values += 4;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 60)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b00100}, 5);
            packed_values += 2;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 60)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b01000100}, 8);
            packed_values += 3;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 61)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b001001}, 6);
            packed_values += 3;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 61)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b010001001}, 9);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 62)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b00101}, 5);
            packed_values += 2;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 62)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b01000101}, 8);
            packed_values += 3;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 63)
            egp.pack(run_length + 2);
            packed_values += 1;
            run_length = 6;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 63)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b010}, 3);
            packed_values += 2;
            run_length = 6;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 64)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b1}, 1);
            packed_values += 2;
            run_length = 6;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 64)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b11}, 2);
            packed_values += 3;
            run_length = 6;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 65)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b100101}, 6);
            packed_values += 3;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 65)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b1100101}, 7);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 66)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b1001001}, 7);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 66)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b11001001}, 8);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 67)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b100100}, 6);
            packed_values += 3;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 67)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b1100100}, 7);
            packed_values += 4;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 68)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b10111}, 5);
            packed_values += 4;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 68)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b110111}, 6);
            packed_values += 5;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 69)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b101111}, 6);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 69)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b1101111}, 7);
            packed_values += 6;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 70)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b1011010}, 7);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 70)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b11011010}, 8);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 71)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b1011}, 4);
            packed_values += 3;
            run_length = 3;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 71)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b11011}, 5);
            packed_values += 4;
            run_length = 3;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 72)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b10101}, 5);
            packed_values += 4;
            run_length = 3;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 72)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b110101}, 6);
            packed_values += 5;
            run_length = 3;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 73)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b10101010}, 8);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 73)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b110101010}, 9);
            packed_values += 6;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 74)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b1010111}, 7);
            packed_values += 6;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 74)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b11010111}, 8);
            packed_values += 7;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 75)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b101011}, 6);
            packed_values += 5;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 75)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b1101011}, 7);
            packed_values += 6;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 76)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b1010010}, 7);
            packed_values += 4;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 76)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b11010010}, 8);
            packed_values += 5;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 77)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b10100101}, 8);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 77)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b110100101}, 9);
            packed_values += 6;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 78)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b1010011}, 7);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 78)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b11010011}, 8);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 79)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b1010}, 4);
            packed_values += 3;
            run_length = 4;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 79)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b11010}, 5);
            packed_values += 4;
            run_length = 4;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 80)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b111}, 3);
            packed_values += 4;
            run_length = 4;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 80)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b1111}, 4);
            packed_values += 5;
            run_length = 4;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 81)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b111011}, 6);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 81)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b1111011}, 7);
            packed_values += 6;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 82)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b1110101}, 7);
            packed_values += 6;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 82)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b11110101}, 8);
            packed_values += 7;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 83)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b111010}, 6);
            packed_values += 5;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 83)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b1111010}, 7);
            packed_values += 6;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 84)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b11111}, 5);
            packed_values += 6;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 84)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b111111}, 6);
            packed_values += 7;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 85)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b111111}, 6);
            packed_values += 7;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 85)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b1111111}, 7);
            packed_values += 8;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 86)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b1111010}, 7);
            packed_values += 6;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 86)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b11111010}, 8);
            packed_values += 7;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 87)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b1111}, 4);
            packed_values += 5;
            run_length = 3;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 87)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b11111}, 5);
            packed_values += 6;
            run_length = 3;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 88)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b11010}, 5);
            packed_values += 4;
            run_length = 3;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 88)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b111010}, 6);
            packed_values += 5;
            run_length = 3;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 89)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b11010010}, 8);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 89)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b111010010}, 9);
            packed_values += 6;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 90)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b1101011}, 7);
            packed_values += 6;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 90)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b11101011}, 8);
            packed_values += 7;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 91)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b110101}, 6);
            packed_values += 5;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 91)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b1110101}, 7);
            packed_values += 6;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 92)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b11011}, 5);
            packed_values += 4;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 92)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b111011}, 6);
            packed_values += 5;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 93)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b110111}, 6);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 93)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b1110111}, 7);
            packed_values += 6;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 94)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b1100100}, 7);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 94)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b11100100}, 8);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 95)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b11}, 2);
            packed_values += 3;
            run_length = 5;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 95)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b111}, 3);
            packed_values += 4;
            run_length = 5;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 96)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b010}, 3);
            packed_values += 2;
            run_length = 5;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 96)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b1010}, 4);
            packed_values += 3;
            run_length = 5;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 97)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b01000100}, 8);
            packed_values += 3;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 97)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b101000100}, 9);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 98)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b0100111}, 7);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 98)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b10100111}, 8);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 99)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b010011}, 6);
            packed_values += 3;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 99)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b1010011}, 7);
            packed_values += 4;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 100)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b0100101}, 7);
            packed_values += 4;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 100)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b10100101}, 8);
            packed_values += 5;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 101)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b01001011}, 8);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 101)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b101001011}, 9);
            packed_values += 6;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 102)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b010010010}, 9);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 102)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b1010010010}, 10);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 103)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b010010}, 6);
            packed_values += 3;
            run_length = 3;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 103)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b1010010}, 7);
            packed_values += 4;
            run_length = 3;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 104)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b01011}, 5);
            packed_values += 4;
            run_length = 3;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 104)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b101011}, 6);
            packed_values += 5;
            run_length = 3;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 105)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b01011010}, 8);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 105)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b101011010}, 9);
            packed_values += 6;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 106)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b0101111}, 7);
            packed_values += 6;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 106)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b10101111}, 8);
            packed_values += 7;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 107)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b010111}, 6);
            packed_values += 5;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 107)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b1010111}, 7);
            packed_values += 6;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 108)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b0101010}, 7);
            packed_values += 4;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 108)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b10101010}, 8);
            packed_values += 5;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 109)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b01010101}, 8);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 109)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b101010101}, 9);
            packed_values += 6;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 110)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b0101011}, 7);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 110)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b10101011}, 8);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 111)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b0101}, 4);
            packed_values += 3;
            run_length = 4;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 111)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b10101}, 5);
            packed_values += 4;
            run_length = 4;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 112)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b011}, 3);
            packed_values += 2;
            run_length = 4;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 112)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b1011}, 4);
            packed_values += 3;
            run_length = 4;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 113)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b011011}, 6);
            packed_values += 3;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 113)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b1011011}, 7);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 114)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b0110101}, 7);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 114)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b10110101}, 8);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 115)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b011010}, 6);
            packed_values += 3;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 115)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b1011010}, 7);
            packed_values += 4;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 116)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b01111}, 5);
            packed_values += 4;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 116)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b101111}, 6);
            packed_values += 5;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 117)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b011111}, 6);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 117)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b1011111}, 7);
            packed_values += 6;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 118)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b0111010}, 7);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 118)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b10111010}, 8);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 119)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b0111}, 4);
            packed_values += 3;
            run_length = 3;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 119)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b10111}, 5);
            packed_values += 4;
            run_length = 3;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 120)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b00100}, 5);
            packed_values += 2;
            run_length = 3;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 120)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b100100}, 6);
            packed_values += 3;
            run_length = 3;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 121)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b00100010}, 8);
            packed_values += 3;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 121)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b100100010}, 9);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 122)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b0010011}, 7);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 122)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b10010011}, 8);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 123)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b001001}, 6);
            packed_values += 3;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 123)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b1001001}, 7);
            packed_values += 4;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 124)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b00101}, 5);
            packed_values += 2;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 124)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b100101}, 6);
            packed_values += 3;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 125)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b001011}, 6);
            packed_values += 3;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 125)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b1001011}, 7);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 126)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b00110}, 5);
            packed_values += 2;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 126)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b100110}, 6);
            packed_values += 3;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 127)
            egp.pack(run_length + 1);
            packed_values += 1;
            run_length = 7;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 127)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b1}, 1);
            packed_values += 2;
            run_length = 7;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 128)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b1}, 1);
            packed_values += 2;
            run_length = 7;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 128)
            egp.pack(run_length + 1);
            packed_values += 1;
            run_length = 7;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 129)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b100110}, 6);
            packed_values += 3;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 129)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b00110}, 5);
            packed_values += 2;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 130)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b1001011}, 7);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 130)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b001011}, 6);
            packed_values += 3;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 131)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b100101}, 6);
            packed_values += 3;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 131)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b00101}, 5);
            packed_values += 2;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 132)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b1001001}, 7);
            packed_values += 4;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 132)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b001001}, 6);
            packed_values += 3;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 133)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b10010011}, 8);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 133)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b0010011}, 7);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 134)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b100100010}, 9);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 134)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b00100010}, 8);
            packed_values += 3;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 135)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b100100}, 6);
            packed_values += 3;
            run_length = 3;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 135)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b00100}, 5);
            packed_values += 2;
            run_length = 3;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 136)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b10111}, 5);
            packed_values += 4;
            run_length = 3;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 136)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b0111}, 4);
            packed_values += 3;
            run_length = 3;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 137)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b10111010}, 8);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 137)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b0111010}, 7);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 138)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b1011111}, 7);
            packed_values += 6;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 138)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b011111}, 6);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 139)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b101111}, 6);
            packed_values += 5;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 139)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b01111}, 5);
            packed_values += 4;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 140)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b1011010}, 7);
            packed_values += 4;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 140)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b011010}, 6);
            packed_values += 3;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 141)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b10110101}, 8);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 141)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b0110101}, 7);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 142)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b1011011}, 7);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 142)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b011011}, 6);
            packed_values += 3;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 143)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b1011}, 4);
            packed_values += 3;
            run_length = 4;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 143)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b011}, 3);
            packed_values += 2;
            run_length = 4;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 144)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b10101}, 5);
            packed_values += 4;
            run_length = 4;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 144)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b0101}, 4);
            packed_values += 3;
            run_length = 4;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 145)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b10101011}, 8);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 145)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b0101011}, 7);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 146)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b101010101}, 9);
            packed_values += 6;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 146)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b01010101}, 8);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 147)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b10101010}, 8);
            packed_values += 5;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 147)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b0101010}, 7);
            packed_values += 4;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 148)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b1010111}, 7);
            packed_values += 6;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 148)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b010111}, 6);
            packed_values += 5;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 149)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b10101111}, 8);
            packed_values += 7;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 149)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b0101111}, 7);
            packed_values += 6;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 150)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b101011010}, 9);
            packed_values += 6;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 150)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b01011010}, 8);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 151)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b101011}, 6);
            packed_values += 5;
            run_length = 3;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 151)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b01011}, 5);
            packed_values += 4;
            run_length = 3;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 152)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b1010010}, 7);
            packed_values += 4;
            run_length = 3;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 152)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b010010}, 6);
            packed_values += 3;
            run_length = 3;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 153)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b1010010010}, 10);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 153)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b010010010}, 9);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 154)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b101001011}, 9);
            packed_values += 6;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 154)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b01001011}, 8);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 155)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b10100101}, 8);
            packed_values += 5;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 155)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b0100101}, 7);
            packed_values += 4;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 156)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b1010011}, 7);
            packed_values += 4;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 156)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b010011}, 6);
            packed_values += 3;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 157)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b10100111}, 8);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 157)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b0100111}, 7);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 158)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b101000100}, 9);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 158)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b01000100}, 8);
            packed_values += 3;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 159)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b1010}, 4);
            packed_values += 3;
            run_length = 5;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 159)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b010}, 3);
            packed_values += 2;
            run_length = 5;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 160)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b111}, 3);
            packed_values += 4;
            run_length = 5;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 160)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b11}, 2);
            packed_values += 3;
            run_length = 5;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 161)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b11100100}, 8);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 161)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b1100100}, 7);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 162)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b1110111}, 7);
            packed_values += 6;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 162)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b110111}, 6);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 163)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b111011}, 6);
            packed_values += 5;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 163)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b11011}, 5);
            packed_values += 4;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 164)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b1110101}, 7);
            packed_values += 6;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 164)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b110101}, 6);
            packed_values += 5;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 165)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b11101011}, 8);
            packed_values += 7;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 165)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b1101011}, 7);
            packed_values += 6;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 166)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b111010010}, 9);
            packed_values += 6;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 166)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b11010010}, 8);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 167)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b111010}, 6);
            packed_values += 5;
            run_length = 3;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 167)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b11010}, 5);
            packed_values += 4;
            run_length = 3;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 168)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b11111}, 5);
            packed_values += 6;
            run_length = 3;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 168)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b1111}, 4);
            packed_values += 5;
            run_length = 3;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 169)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b11111010}, 8);
            packed_values += 7;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 169)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b1111010}, 7);
            packed_values += 6;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 170)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b1111111}, 7);
            packed_values += 8;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 170)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b111111}, 6);
            packed_values += 7;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 171)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b111111}, 6);
            packed_values += 7;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 171)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b11111}, 5);
            packed_values += 6;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 172)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b1111010}, 7);
            packed_values += 6;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 172)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b111010}, 6);
            packed_values += 5;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 173)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b11110101}, 8);
            packed_values += 7;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 173)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b1110101}, 7);
            packed_values += 6;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 174)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b1111011}, 7);
            packed_values += 6;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 174)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b111011}, 6);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 175)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b1111}, 4);
            packed_values += 5;
            run_length = 4;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 175)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b111}, 3);
            packed_values += 4;
            run_length = 4;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 176)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b11010}, 5);
            packed_values += 4;
            run_length = 4;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 176)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b1010}, 4);
            packed_values += 3;
            run_length = 4;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 177)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b11010011}, 8);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 177)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b1010011}, 7);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 178)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b110100101}, 9);
            packed_values += 6;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 178)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b10100101}, 8);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 179)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b11010010}, 8);
            packed_values += 5;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 179)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b1010010}, 7);
            packed_values += 4;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 180)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b1101011}, 7);
            packed_values += 6;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 180)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b101011}, 6);
            packed_values += 5;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 181)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b11010111}, 8);
            packed_values += 7;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 181)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b1010111}, 7);
            packed_values += 6;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 182)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b110101010}, 9);
            packed_values += 6;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 182)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b10101010}, 8);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 183)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b110101}, 6);
            packed_values += 5;
            run_length = 3;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 183)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b10101}, 5);
            packed_values += 4;
            run_length = 3;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 184)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b11011}, 5);
            packed_values += 4;
            run_length = 3;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 184)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b1011}, 4);
            packed_values += 3;
            run_length = 3;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 185)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b11011010}, 8);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 185)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b1011010}, 7);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 186)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b1101111}, 7);
            packed_values += 6;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 186)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b101111}, 6);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 187)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b110111}, 6);
            packed_values += 5;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 187)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b10111}, 5);
            packed_values += 4;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 188)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b1100100}, 7);
            packed_values += 4;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 188)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b100100}, 6);
            packed_values += 3;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 189)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b11001001}, 8);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 189)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b1001001}, 7);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 190)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b1100101}, 7);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 190)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b100101}, 6);
            packed_values += 3;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 191)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b11}, 2);
            packed_values += 3;
            run_length = 6;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 191)
            egp.pack(run_length + 1);
            egp.pack(std::uint64_t{0b1}, 1);
            packed_values += 2;
            run_length = 6;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 192)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b010}, 3);
            packed_values += 2;
            run_length = 6;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 192)
            egp.pack(run_length + 2);
            packed_values += 1;
            run_length = 6;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 193)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b01000101}, 8);
            packed_values += 3;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 193)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b00101}, 5);
            packed_values += 2;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 194)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b010001001}, 9);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 194)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b001001}, 6);
            packed_values += 3;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 195)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b01000100}, 8);
            packed_values += 3;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 195)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b00100}, 5);
            packed_values += 2;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 196)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b0100111}, 7);
            packed_values += 4;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 196)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b0111}, 4);
            packed_values += 3;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 197)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b01001111}, 8);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 197)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b01111}, 5);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 198)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b010011010}, 9);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 198)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b011010}, 6);
            packed_values += 3;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 199)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b010011}, 6);
            packed_values += 3;
            run_length = 3;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 199)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b011}, 3);
            packed_values += 2;
            run_length = 3;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 200)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b0100101}, 7);
            packed_values += 4;
            run_length = 3;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 200)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b0101}, 4);
            packed_values += 3;
            run_length = 3;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 201)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b0100101010}, 10);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 201)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b0101010}, 7);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 202)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b010010111}, 9);
            packed_values += 6;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 202)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b010111}, 6);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 203)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b01001011}, 8);
            packed_values += 5;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 203)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b01011}, 5);
            packed_values += 4;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 204)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b010010010}, 9);
            packed_values += 4;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 204)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b010010}, 6);
            packed_values += 3;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 205)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b0100100101}, 10);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 205)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b0100101}, 7);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 206)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b010010011}, 9);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 206)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b010011}, 6);
            packed_values += 3;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 207)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b010010}, 6);
            packed_values += 3;
            run_length = 4;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 207)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b010}, 3);
            packed_values += 2;
            run_length = 4;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 208)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b01011}, 5);
            packed_values += 4;
            run_length = 4;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 208)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b11}, 2);
            packed_values += 3;
            run_length = 4;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 209)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b01011011}, 8);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 209)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b11011}, 5);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 210)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b010110101}, 9);
            packed_values += 6;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 210)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b110101}, 6);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 211)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b01011010}, 8);
            packed_values += 5;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 211)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b11010}, 5);
            packed_values += 4;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 212)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b0101111}, 7);
            packed_values += 6;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 212)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b1111}, 4);
            packed_values += 5;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 213)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b01011111}, 8);
            packed_values += 7;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 213)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b11111}, 5);
            packed_values += 6;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 214)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b010111010}, 9);
            packed_values += 6;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 214)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b111010}, 6);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 215)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b010111}, 6);
            packed_values += 5;
            run_length = 3;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 215)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b111}, 3);
            packed_values += 4;
            run_length = 3;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 216)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b0101010}, 7);
            packed_values += 4;
            run_length = 3;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 216)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b1010}, 4);
            packed_values += 3;
            run_length = 3;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 217)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b0101010010}, 10);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 217)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b1010010}, 7);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 218)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b010101011}, 9);
            packed_values += 6;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 218)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b101011}, 6);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 219)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b01010101}, 8);
            packed_values += 5;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 219)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b10101}, 5);
            packed_values += 4;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 220)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b0101011}, 7);
            packed_values += 4;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 220)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b1011}, 4);
            packed_values += 3;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 221)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b01010111}, 8);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 221)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b10111}, 5);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 222)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b010100100}, 9);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 222)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b100100}, 6);
            packed_values += 3;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 223)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b0101}, 4);
            packed_values += 3;
            run_length = 5;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 223)
            egp.pack(run_length + 2);
            egp.pack(std::uint64_t{0b1}, 1);
            packed_values += 2;
            run_length = 5;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 224)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b011}, 3);
            packed_values += 2;
            run_length = 5;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 224)
            egp.pack(run_length + 3);
            packed_values += 1;
            run_length = 5;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 225)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b01100100}, 8);
            packed_values += 3;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 225)
            egp.pack(run_length + 3);
            egp.pack(std::uint64_t{0b00100}, 5);
            packed_values += 2;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 226)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b0110111}, 7);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 226)
            egp.pack(run_length + 3);
            egp.pack(std::uint64_t{0b0111}, 4);
            packed_values += 3;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 227)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b011011}, 6);
            packed_values += 3;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 227)
            egp.pack(run_length + 3);
            egp.pack(std::uint64_t{0b011}, 3);
            packed_values += 2;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 228)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b0110101}, 7);
            packed_values += 4;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 228)
            egp.pack(run_length + 3);
            egp.pack(std::uint64_t{0b0101}, 4);
            packed_values += 3;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 229)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b01101011}, 8);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 229)
            egp.pack(run_length + 3);
            egp.pack(std::uint64_t{0b01011}, 5);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 230)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b011010010}, 9);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 230)
            egp.pack(run_length + 3);
            egp.pack(std::uint64_t{0b010010}, 6);
            packed_values += 3;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 231)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b011010}, 6);
            packed_values += 3;
            run_length = 3;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 231)
            egp.pack(run_length + 3);
            egp.pack(std::uint64_t{0b010}, 3);
            packed_values += 2;
            run_length = 3;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 232)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b01111}, 5);
            packed_values += 4;
            run_length = 3;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 232)
            egp.pack(run_length + 3);
            egp.pack(std::uint64_t{0b11}, 2);
            packed_values += 3;
            run_length = 3;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 233)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b01111010}, 8);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 233)
            egp.pack(run_length + 3);
            egp.pack(std::uint64_t{0b11010}, 5);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 234)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b0111111}, 7);
            packed_values += 6;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 234)
            egp.pack(run_length + 3);
            egp.pack(std::uint64_t{0b1111}, 4);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 235)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b011111}, 6);
            packed_values += 5;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 235)
            egp.pack(run_length + 3);
            egp.pack(std::uint64_t{0b111}, 3);
            packed_values += 4;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 236)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b0111010}, 7);
            packed_values += 4;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 236)
            egp.pack(run_length + 3);
            egp.pack(std::uint64_t{0b1010}, 4);
            packed_values += 3;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 237)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b01110101}, 8);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 237)
            egp.pack(run_length + 3);
            egp.pack(std::uint64_t{0b10101}, 5);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 238)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b0111011}, 7);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 238)
            egp.pack(run_length + 3);
            egp.pack(std::uint64_t{0b1011}, 4);
            packed_values += 3;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 239)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b0111}, 4);
            packed_values += 3;
            run_length = 4;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 239)
            egp.pack(run_length + 3);
            egp.pack(std::uint64_t{0b1}, 1);
            packed_values += 2;
            run_length = 4;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 240)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b00100}, 5);
            packed_values += 2;
            run_length = 4;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 240)
            egp.pack(run_length + 4);
            packed_values += 1;
            run_length = 4;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 241)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b00100011}, 8);
            packed_values += 3;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 241)
            egp.pack(run_length + 4);
            egp.pack(std::uint64_t{0b011}, 3);
            packed_values += 2;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 242)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b001000101}, 9);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 242)
            egp.pack(run_length + 4);
            egp.pack(std::uint64_t{0b0101}, 4);
            packed_values += 3;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 243)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b00100010}, 8);
            packed_values += 3;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 243)
            egp.pack(run_length + 4);
            egp.pack(std::uint64_t{0b010}, 3);
            packed_values += 2;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 244)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b0010011}, 7);
            packed_values += 4;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 244)
            egp.pack(run_length + 4);
            egp.pack(std::uint64_t{0b11}, 2);
            packed_values += 3;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 245)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b00100111}, 8);
            packed_values += 5;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 245)
            egp.pack(run_length + 4);
            egp.pack(std::uint64_t{0b111}, 3);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 246)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b001001010}, 9);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 246)
            egp.pack(run_length + 4);
            egp.pack(std::uint64_t{0b1010}, 4);
            packed_values += 3;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 247)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b001001}, 6);
            packed_values += 3;
            run_length = 3;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 247)
            egp.pack(run_length + 4);
            egp.pack(std::uint64_t{0b1}, 1);
            packed_values += 2;
            run_length = 3;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 248)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b00101}, 5);
            packed_values += 2;
            run_length = 3;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 248)
            egp.pack(run_length + 5);
            packed_values += 1;
            run_length = 3;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 249)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b00101010}, 8);
            packed_values += 3;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 249)
            egp.pack(run_length + 5);
            egp.pack(std::uint64_t{0b010}, 3);
            packed_values += 2;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 250)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b0010111}, 7);
            packed_values += 4;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 250)
            egp.pack(run_length + 5);
            egp.pack(std::uint64_t{0b11}, 2);
            packed_values += 3;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 251)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b001011}, 6);
            packed_values += 3;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 251)
            egp.pack(run_length + 5);
            egp.pack(std::uint64_t{0b1}, 1);
            packed_values += 2;
            run_length = 2;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 252)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b00110}, 5);
            packed_values += 2;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 252)
            egp.pack(run_length + 6);
            packed_values += 1;
            run_length = 2;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 253)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b001101}, 6);
            packed_values += 3;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 253)
            egp.pack(run_length + 6);
            egp.pack(std::uint64_t{0b1}, 1);
            packed_values += 2;
            run_length = 1;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(0, 254)
            egp.pack(run_length);
            egp.pack(std::uint64_t{0b00111}, 5);
            packed_values += 2;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(1, 254)
            egp.pack(run_length + 7);
            packed_values += 1;
            run_length = 1;
            state = std::uint8_t{0};
            GOTO_LOOP;
        TARGET(0, 255)
            egp.pack(run_length);
            packed_values += 1;
            run_length = 8;
            state = std::uint8_t{1};
            GOTO_LOOP;
        TARGET(1, 255)
            run_length += 8;
            state = std::uint8_t{1};
            GOTO_LOOP;

        #ifndef COMPUTED_GOTO
        }
        #endif
    }

    //Store last run-length
    egp.pack(run_length);
    egp.set_packed_values_count(packed_values+1);
}

int main(int argc, char ** args) {

    if(argc != 3)
    {
        std::cout << "Usage: rle_egp <input> <output>\n" << std::endl;
        return 1;
    }

    std::string input_filename = args[1];
    std::string output_filename = args[2];

    EliasGammaPacker egp;

    int fd = open(input_filename.c_str(), O_RDONLY);
    if (fd == -1)
        throw std::runtime_error("main : couldn't open file '" + input_filename + "'");


    std::size_t file_size = lseek(fd, 0, SEEK_END);

    if(file_size == 0)
        throw std::runtime_error("main : file was empty");

    void* map = mmap(nullptr, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (map == MAP_FAILED) {
        throw std::runtime_error("main : mmap initialization failed");
    }

    posix_madvise(map, file_size, MADV_SEQUENTIAL);

    const std::uint8_t* const data = static_cast<const std::uint8_t* const>(map);

    egp.set_first_bit(data[0] >> 7);
    egp.resize(1 << 26); //Set initial vector of bytes to 64MB
    egp.set_file_size(file_size);
    
    BitRunDFA(egp, data, file_size);

    munmap(map, file_size);
    close(fd);

    egp.serialize(output_filename);

    return 0;
}
