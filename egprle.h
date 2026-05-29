#ifndef EGPRLE_H
#define EGPRLE_H

#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <vector>
#include <cstdlib>
#include <immintrin.h>
#include <emmintrin.h>

#include "utils.h"
#include "circular_buffer.h"

typedef std::uint32_t run_length_t;
#define NB_RUNS 16

#include <iostream>

namespace EliasGammaPacker
{
    class EGPRLE
    {
        private:
            //Bit-run DFA
            std::size_t dfa_pos;
            run_length_t dfa_run_length;
            std::uint8_t dfa_state;

            //Run-length buffer
            CircularDoubleBuffer<run_length_t, NB_RUNS> buffer;
        public:

            //RLE compression, can be worse than uncompressed data. But in case of RLE non-compressible data, data is stored uncompressed (TODO TO BE DONE)
            //WARNING: tard-implemented
            static std::size_t compressBound(std::size_t data_size)
            {
                return 2*data_size;
            }

            void BitRunDFA(const std::uint8_t * const data, std::size_t length);

            //dst must be 32-byte aligned (e.g. alignas(32))
            std::size_t encode(char* dst, std::size_t dst_size, const char* src, std::size_t src_size)
            {
                if(reinterpret_cast<std::uintptr_t>(dst) % 32 != 0)
                    throw std::runtime_error("egprle :: encode : destination is not 32-byte aligned.");

                std::uint8_t* dst_pos = reinterpret_cast<std::uint8_t*>(dst);
                std::uint8_t* const dst_end = dst_pos + dst_size;

                //Zero destination
                std::memset(dst, 0, dst_size);

                constexpr int lane_width = 256;
                constexpr int lane_nb = 2;
                constexpr int sublane_width = lane_width * lane_nb / NB_RUNS;
                constexpr int selector_width = sublane_width;
                constexpr int POFFSET = lane_width / sublane_width;

                using selector_t = compile_time_uint_t<selector_width>;
                typedef union { __m256i v; std::uint8_t bytes[lane_width/8]; } payload_t;

                selector_t* selector = reinterpret_cast<selector_t*>(dst_pos);
                payload_t* payload1 = reinterpret_cast<payload_t*>(dst_pos + sizeof(selector_t));
                payload_t* payload2 = reinterpret_cast<payload_t*>(dst_pos + sizeof(selector_t) + sizeof(payload_t));

                int remaining_bits = sublane_width;
                std::size_t offset = 0;

                dfa_pos = 0;
                dfa_run_length = 0;
                buffer.clear();

                do
                {
                    BitRunDFA(reinterpret_cast<const std::uint8_t*>(src), src_size);

                    const __m256i values1 = _mm256_load_si256((const __m256i*)buffer.ptr());
                    const __m256i values2 = _mm256_load_si256((const __m256i*)(buffer.ptr()+POFFSET));

                    //Process 16 integers
                    //SIMD code Group Elias Gamma here 2x AVX2 (16x32)

                    std::uint8_t frame_width = 0;
                    
                    //Get width (log2+1) of 16 integers
                    {
                        run_length_t merge = run_length_t{1};
                        
                        int i = 0;
                        for(; i < NB_RUNS; ++i)
                            merge |= buffer[i]; //Possible to vectorize here

                        frame_width = log2_64(merge)+1;
                    }

                    //Enough space
                    if(frame_width <= remaining_bits)
                    {
                        //Update selector
                        *selector |= unary<selector_t>(frame_width-1) << offset;

                        payload1->v = _mm256_or_si256(_mm256_slli_epi32(values1, offset), payload1->v);
                        payload2->v = _mm256_or_si256(_mm256_slli_epi32(values2, offset), payload2->v);

                        offset += frame_width;
                        remaining_bits -= frame_width;
                    }
                    //Need to split/move to next lane
                    else 
                    {
                        int split_frame_width = frame_width - remaining_bits;

                        if(remaining_bits != 0)
                        {
                            const __m256i split1_mask = _mm256_load_si256((const __m256i*)mask_lsb_not[split_frame_width]);
            
                            //Selector low bits go to current split (split1) - nothing to do

                            //Frame high bits go to current split (split1)
                            int shift = sublane_width - frame_width;
                            payload1->v = _mm256_or_si256(_mm256_slli_epi32(_mm256_and_si256(values1, split1_mask), shift), payload1->v);
                            payload2->v = _mm256_or_si256(_mm256_slli_epi32(_mm256_and_si256(values2, split1_mask), shift), payload2->v);
                        }

                        //Check memory out of range
                        dst_pos += sizeof(selector_t) + 2 * sizeof(payload_t);
                        if(dst_pos + sizeof(selector_t) + 2*sizeof(payload_t) >= dst_end)
                            throw std::runtime_error("egprle :: encode : Out of range");

                        //Update pointers
                        selector = reinterpret_cast<selector_t*>(dst_pos);
                        payload1 = reinterpret_cast<payload_t*>(dst_pos + sizeof(selector_t));
                        payload2 = reinterpret_cast<payload_t*>(dst_pos + sizeof(selector_t) + sizeof(payload_t));

                        //Selector high bits go to next split (split2)
                        *selector |= unary<selector_t>(split_frame_width-1);

                        const __m256i split2_mask = _mm256_load_si256((const __m256i*)mask_lsb[split_frame_width]);
                        
                        //Frame low bits go to next split (split2)
                        payload1->v = _mm256_and_si256(values1, split2_mask);
                        payload2->v = _mm256_and_si256(values2, split2_mask);

                        //Reset offset and remaining_bits variables
                        offset = split_frame_width;
                        remaining_bits = sublane_width - split_frame_width;
                    }

                    //Cycle buffer offset
                    buffer.cycle();
                }
                while(dfa_pos < src_size);

                return //relative position + selector size + 2*payload size
                 + reinterpret_cast<const char*>(dst_pos)
                 - reinterpret_cast<const char*>(dst)
                 + sizeof(selector_t)
                 + sizeof(payload_t)
                 + sizeof(payload_t);
            }

            std::size_t decode(char* dst, std::size_t dst_size, const char* src, std::size_t src_size)
            {
                /*std::size_t bit_pos = 0;
                std::size_t run_length = 0;
                

                //Loop to decompress 
                if(!egp.get_first_bit() && nb_packed_values > 0)
                {
                    bit_pos = egp.unpack(); //Unpack first run of 0s
                    i = 1;
                }

                const std::uint8_t FF = 0xFF;

                for(; i+1 < nb_packed_values; i += 2)
                {
                    run_length = egp.unpack(); //Get first bit run of 1s

                    // Handle first unaligned partial byte if any
                    if(bit_pos % 8 != 0)
                    {   
                        if(run_length + (bit_pos % 8) < 8)
                        {
                            //Update first unaligned partial byte
                            map[bit_pos/8] |= (((std::uint8_t{1}) << run_length) - 1) << (8 - (bit_pos % 8) - run_length);

                            //Update bit position to next byte
                            bit_pos += run_length;

                            //Update run-length
                            run_length = 0;

                        }
                        else
                        {
                            //Update first unaligned partial byte
                            map[bit_pos/8] |= FF >> (bit_pos % 8);
                            
                            //Update run-length
                            run_length -= 8 - (bit_pos % 8);

                            //Update bit position to next byte
                            bit_pos = bit_pos/8*8 + 8;
                        }
                    }

                    // Handle aligned bytes 
                    for(std::uint64_t j = 0; j < run_length / 8; ++j, bit_pos += 8)
                        map[bit_pos/8] = FF;

                    // Handle last unaligned partial byte if any
                    if(run_length % 8 != 0)
                        map[bit_pos/8] = FF << (8 - run_length % 8);

                    //Update current bit position by the previously number of added 1s and add also the of next 0s
                    bit_pos += (run_length % 8) + egp.unpack();
                }

                //Handle last run of 1s 
                if(egp.get_first_bit() == nb_packed_values % 2)
                {
                    run_length = egp.unpack(); //Get first bit run of 1s
                        
                    // Handle first unaligned partial byte if any
                    if(bit_pos % 8 != 0)
                    {   
                        if(run_length + (bit_pos % 8) < 8)
                        {
                            //Update first unaligned partial byte
                            map[bit_pos/8] |= (((std::uint8_t{1}) << run_length) - 1) << (8 - (bit_pos % 8) - run_length);

                            //Update bit position to next byte
                            bit_pos += run_length;

                            //Update run-length
                            run_length = 0;
                        }
                        else
                        {
                            //Update first unaligned partial byte
                            map[bit_pos/8] |= FF >> (bit_pos % 8);
                            
                            //Update run-length
                            run_length -= 8 - (bit_pos % 8);

                            //Update bit position to next byte
                            bit_pos = bit_pos/8*8 + 8;
                        }
                    }
                    
                    // Handle aligned bytes 
                    for(std::uint64_t j = 0; j < run_length / 8; ++j, bit_pos += 8)
                        map[bit_pos/8] = FF;

                    // Handle last unaligned partial byte if any
                    if(run_length % 8 != 0)
                        map[bit_pos/8] = FF << (8 - run_length % 8);

                    //bit_pos += run_length + egp.unpack(); //Update current bit position by the previously number of added 1s and add also the of next 0s
                }*/

                return 0;
            }


    };
}

#endif