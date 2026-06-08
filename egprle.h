#ifndef EGP_RLE_H
#define EGP_RLE_H

#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <vector>
#include <cstdlib>
#include <immintrin.h>
#include <emmintrin.h>

#include "utils.h"
#include "types.h"
#include "circular_buffer.h"

#include <iostream>

namespace EliasGammaPacker
{
    static const decode_partial_struct_t decode_partial_init_value = {
        .mask = {0},
        .selector = {0},
        .payload1 = {0},
        .payload2 = {0},
        .remaining_bits = {0},
        .frame_width = sublane_width,
        .bit_pos = {0},
        .next_entry_point = entry_point_t::init
    };

    class EGPRLE
    {
        private:
            //Bit-run DFA data
            bit_run_dfa_struct_t dfa_data;
            
            //Partial decode data
            decode_partial_struct_t decode_partial_data;

            //Run-length buffer
            CircularDoubleBuffer<run_length_t, nb_runs> buffer;

            //Metadata
            metadata_struct_t meta;

        public:

            //RLE compression, can be worse than uncompressed data. But in case of RLE non-compressible data, data is stored uncompressed (TODO TO BE DONE)
            //WARNING: tard-implemented
            static std::size_t compressBound(std::size_t size)
            {
                return size + sizeof(metadata_t) + sizeof(selector_t) + 2*sizeof(payload_t<sublane_width>);
            }

            void BitRunDFA(const std::uint8_t * const data, std::size_t length);

            static metadata_struct_t read_meta(const char* src)
            {
                metadata_t x = *reinterpret_cast<const metadata_t*>(src);
                std::uint8_t sbv = x & 1;
                std::uint8_t ir = (x >> 1) & 1;

                return metadata_struct_t{ .out_size = x >> 2, .is_raw = ir, .starting_bit_value = sbv };
            }

            static void write_meta(char* dst, metadata_struct_t meta)
            {
                metadata_t x = (meta.out_size << 2) | (meta.is_raw << 1) | (meta.starting_bit_value);
                std::memcpy(dst, reinterpret_cast<char*>(x), sizeof(metadata_t));
            }

            //dst must be 32-byte aligned (e.g. alignas(32))
            std::size_t encode(char* dst, std::size_t dst_size, const char* src, std::size_t src_size)
            {
                if(reinterpret_cast<std::uintptr_t>(dst) % 32 != 0)
                    throw std::runtime_error("egprle :: encode : destination is not 32-byte aligned.");

                std::uint8_t* dst_pos = reinterpret_cast<std::uint8_t*>(dst)+sizeof(metadata_t);
                std::uint8_t* const dst_end = dst_pos + dst_size;

                std::uint8_t starting_bit_value = *((const uint8_t*)src) >> 7 & 1;

                //Zero destination
                std::memset(dst, 0, dst_size);


                //Need 32-byte alignment
                selector_t* selector = reinterpret_cast<selector_t*>(dst_pos);
                payload_t<sublane_width>* payload1 = reinterpret_cast<payload_t<sublane_width>*>(dst_pos + sizeof(selector_t));
                payload_t<sublane_width>* payload2 = reinterpret_cast<payload_t<sublane_width>*>(dst_pos + sizeof(selector_t) + sizeof(payload_t<sublane_width>));

                int remaining_bits = sublane_width;
                std::size_t offset = 0;
                std::uint8_t frame_width;

                dfa_data.dfa_pos = 0;
                dfa_data.dfa_run_length = 0;
                buffer.clear();

                const __m256i* const v1 = (const __m256i*)(buffer.ptr());
                const __m256i* const v2 = (const __m256i*)(buffer.ptr() + values_offset);

                do
                {
                    BitRunDFA(reinterpret_cast<const std::uint8_t*>(src), src_size);

                    //_mm256_sub_epi32(*v1, ones); //Done during BitRunDFA

                    //Process 'nb_runs' integers
                    //SIMD code Group Elias Gamma here 2x AVX2 ('nb_runs'x32)
                    //Get width (log2+1) of 'nb_runs' integers
                    {
                        run_length_t merge = run_length_t{1};
                        
                        int i = 0;
                        for(; i < nb_runs; ++i)
                            merge |= buffer[i]; //Possible to vectorize here

                        frame_width = log2_64(merge)+1;
                    }

                    //Enough space
                    if(frame_width <= remaining_bits)
                    {
                        //Update selector
                        *selector |= unary<selector_t>(frame_width-1) << offset;

                        payload1->v = _mm256_or_si256(_mm256_slli_epi32(*v1, offset), payload1->v);
                        payload2->v = _mm256_or_si256(_mm256_slli_epi32(*v2, offset), payload2->v);

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
                            payload1->v = _mm256_or_si256(_mm256_slli_epi32(_mm256_and_si256(*v1, split1_mask), shift), payload1->v);
                            payload2->v = _mm256_or_si256(_mm256_slli_epi32(_mm256_and_si256(*v2, split1_mask), shift), payload2->v);
                        }

                        //Check memory out of range
                        dst_pos += sizeof(selector_t) + 2 * sizeof(payload_t<sublane_width>);
                        if(dst_pos + sizeof(selector_t) + 2*sizeof(payload_t<sublane_width>) >= dst_end)
                            throw std::runtime_error("egprle :: encode : Out of range");

                        //Update pointers (need 32-byte alignment)
                        selector = reinterpret_cast<selector_t*>(dst_pos);
                        payload1 = reinterpret_cast<payload_t<sublane_width>*>(dst_pos + sizeof(selector_t));
                        payload2 = reinterpret_cast<payload_t<sublane_width>*>(dst_pos + sizeof(selector_t) + sizeof(payload_t<sublane_width>));

                        //Selector high bits go to next split (split2)
                        *selector |= unary<selector_t>(split_frame_width-1);

                        const __m256i split2_mask = _mm256_load_si256((const __m256i*)mask_lsb[split_frame_width]);
                        
                        //Frame low bits go to next split (split2)
                        payload1->v = _mm256_and_si256(*v1, split2_mask);
                        payload2->v = _mm256_and_si256(*v2, split2_mask);

                        //Reset offset and remaining_bits variables
                        offset = split_frame_width;
                        remaining_bits = sublane_width - split_frame_width;
                    }

                    //Cycle buffer offset
                    buffer.cycle();
                }
                while(dfa_data.dfa_pos < src_size);

                if((dst_pos - reinterpret_cast<std::uint8_t*>(dst)) >= src_size)
                {
                    //Save as raw
                    std::memcpy(dst + sizeof(metadata_t), src, src_size);
                    meta = { .out_size = src_size, .is_raw = 1, .starting_bit_value = starting_bit_value };
                }
                else
                    meta = { .out_size = src_size, .is_raw = 0, .starting_bit_value = starting_bit_value };

                write_meta(dst, meta);

                return //relative position + selector size + 2*payload size
                 + reinterpret_cast<const char*>(dst_pos)
                 - reinterpret_cast<const char*>(dst)
                 + sizeof(selector_t)
                 + sizeof(payload_t<sublane_width>)
                 + sizeof(payload_t<sublane_width>);
            }

            std::size_t decode(char* dst, std::size_t dst_size, const char* src, std::size_t src_size)
            {
                meta = read_meta(src);

                const std::uint8_t* src_pos = reinterpret_cast<const std::uint8_t*>(src+sizeof(metadata_t));

                if(meta.is_raw)
                {
                    std::memcpy(dst, src_pos, meta.out_size);
                    return meta.out_size;
                }

                int constexpr read_size = sizeof(selector_t) + 2*sizeof(payload_t<sublane_width>);

                const std::uint8_t* const src_end = src_pos + src_size;

                std::uint8_t* dst_pos = reinterpret_cast<std::uint8_t*>(dst);
                std::uint8_t* const dst_end = dst_pos + dst_size;

                const __m256i ones = _mm256_set1_epi32(1);

                std::size_t bit_pos = 0;
                const std::size_t bit_end = dst_size*8;

                //Zero destination
                std::memset(dst, 0, dst_size);

                selector_t selector{0};
                payload_t<sublane_width> payload1, payload2;

                alignas(32) run_length_t values[nb_runs] = {0};
                __m256i* const v1 = (__m256i*)values;
                __m256i* const v2 = (__m256i*)(values+values_offset);

                int remaining_bits;
                int frame_width;

                while(bit_pos < bit_end)
                {
                    remaining_bits = sublane_width;

                    selector = *reinterpret_cast<const selector_t*>(src_pos);

                    payload1.v = _mm256_load_si256(reinterpret_cast<const __m256i*>(src_pos + sizeof(selector_t)));
                    payload2.v = _mm256_load_si256(reinterpret_cast<const __m256i*>(src_pos + sizeof(selector_t) + sizeof(payload_t<sublane_width>)));
                    
                    frame_width = trailing_zeroes(selector) + 1;
                    const __m256i mask = _mm256_load_si256((const __m256i*)mask_lsb[frame_width]);

                    *v1 = _mm256_add_epi32(_mm256_or_si256(_mm256_slli_epi32(*v1, frame_width), _mm256_and_si256(payload1.v, mask)), ones);
                    *v2 = _mm256_add_epi32(_mm256_or_si256(_mm256_slli_epi32(*v2, frame_width), _mm256_and_si256(payload2.v, mask)), ones);
                    decode_bit_runs(dst_pos, bit_pos, bit_end, values, nb_runs, meta.starting_bit_value);

                    payload1.v = _mm256_srli_epi32(payload1.v, frame_width);
                    payload2.v = _mm256_srli_epi32(payload2.v, frame_width);

                    selector >>= frame_width;
                    remaining_bits -= frame_width;
                    
                    while(selector != 0)
                    {
                        frame_width = trailing_zeroes(selector) + 1;
                        const __m256i mask = _mm256_load_si256((const __m256i*)mask_lsb[frame_width]);

                        *v1 = _mm256_add_epi32(_mm256_and_si256(payload1.v, mask), ones);
                        *v2 = _mm256_add_epi32(_mm256_and_si256(payload2.v, mask), ones);
                        decode_bit_runs(dst_pos, bit_pos, bit_end, values, nb_runs, meta.starting_bit_value);
                        
                        payload1.v = _mm256_srli_epi32(payload1.v, frame_width);
                        payload2.v = _mm256_srli_epi32(payload2.v, frame_width);

                        selector >>= frame_width;
                        remaining_bits -= frame_width; //Note: can't be negative as sum of frame_width is less than or equal to 32
                    }
                    
                    //Handle overlap (even if no overlap, "if" would slow process)
                    //Extract values from payloads (last frame)
                    *v1 = payload1.v;
                    *v2 = payload2.v;
                    
                    //Move cursor
                    src_pos += read_size;
                }

                return 0;
            }

            std::size_t decode_partial(char* dst, std::size_t dst_size, const char* src, std::size_t src_size, std::size_t decode_until_size)
            {
                decode_partial_struct_t& d = decode_partial_data;

                alignas(32) run_length_t values[nb_runs] = {0};
                __m256i* const v1 = (__m256i*)(values);
                __m256i* const v2 = (__m256i*)(values+values_offset);
                
                std::size_t nb_values = 0;

                int constexpr read_size = sizeof(selector_t) + 2*sizeof(payload_t<sublane_width>);

                const std::uint8_t* src_pos = reinterpret_cast<const std::uint8_t*>(src+sizeof(metadata_t));
                const std::uint8_t* const src_end = reinterpret_cast<const std::uint8_t*>(src) + src_size;

                std::uint8_t* dst_pos = reinterpret_cast<std::uint8_t*>(dst);
                std::uint8_t* const dst_end = reinterpret_cast<std::uint8_t*>(dst) + dst_size;

                const __m256i ones = _mm256_set1_epi32(1);

                const std::size_t bit_end = decode_until_size*8;

                switch(d.next_entry_point)
                {
                    case entry_point_t::inner:
                        goto egp_inner;
                    case entry_point_t::begin:
                        goto egp_begin;
                    case entry_point_t::init:
                        
                        //Zero destination
                        std::memset(dst, 0, dst_size);
                        meta = read_meta(src);
                        break;
                    default:
                        throw std::runtime_error("egprle : decode_partial : unexpected entry point");
                }
                
                while(src_pos + read_size <= src_end)
                {
                    d.remaining_bits = sublane_width;

                    d.selector = *reinterpret_cast<const selector_t*>(src_pos);

                    d.payload1.v = _mm256_load_si256(reinterpret_cast<const __m256i*>(src_pos + sizeof(selector_t)));
                    d.payload2.v = _mm256_load_si256(reinterpret_cast<const __m256i*>(src_pos + sizeof(selector_t) + sizeof(payload_t<sublane_width>)));

                    d.frame_width = trailing_zeroes(d.selector) + 1;
                    d.mask = _mm256_load_si256((const __m256i*)mask_lsb[d.frame_width]);

                    *v1 = _mm256_add_epi32(_mm256_or_si256(_mm256_slli_epi32(*v1, d.frame_width), _mm256_and_si256(d.payload1.v, d.mask)), ones);
                    *v2 = _mm256_add_epi32(_mm256_or_si256(_mm256_slli_epi32(*v2, d.frame_width), _mm256_and_si256(d.payload2.v, d.mask)), ones);

                    d.next_entry_point = entry_point_t::begin;
                    decode_bit_runs(dst_pos, d.bit_pos, bit_end, values, nb_runs, meta.starting_bit_value);
                egp_begin:
                    d.payload1.v = _mm256_srli_epi32(d.payload1.v, d.frame_width);
                    d.payload2.v = _mm256_srli_epi32(d.payload2.v, d.frame_width);

                    d.selector >>= d.frame_width;
                    d.remaining_bits -= d.frame_width;
                    
                    while(d.selector != 0)
                    {
                        d.frame_width = trailing_zeroes(d.selector) + 1;
                        d.mask = _mm256_load_si256((const __m256i*)mask_lsb[d.frame_width]);

                        *v1 = _mm256_add_epi32(_mm256_and_si256(d.payload1.v, d.mask), ones);
                        *v2 = _mm256_add_epi32(_mm256_and_si256(d.payload2.v, d.mask), ones);
                        d.next_entry_point = entry_point_t::inner;
                        decode_bit_runs(dst_pos, d.bit_pos, bit_end, values, nb_runs, meta.starting_bit_value);
                egp_inner:
                        d.payload1.v = _mm256_srli_epi32(d.payload1.v, d.frame_width);
                        d.payload2.v = _mm256_srli_epi32(d.payload2.v, d.frame_width);

                        d.selector >>= d.frame_width;
                        d.remaining_bits -= d.frame_width; //Note: can't be negative as sum of frame_width is less than or equal to 32
                    }
                    
                    //Handle overlap (even if no overlap, "if" would slow process)
                    //Extract values from payloads (last frame)
                    *v1 = d.payload1.v;
                    *v2 = d.payload2.v;
                    
                    //Move cursor
                    src_pos += read_size;
                }
            }

            //Not implemented
            std::size_t decode_partial_backward(char* dst, std::size_t dst_size, const char* src, std::size_t src_size, std::size_t decode_until_size);

            //Encode 'nb_runs' runs of bits into 'dst', return current byte position
            static void decode_bit_runs(std::uint8_t* const dst, std::size_t& bit_pos, const std::size_t bit_end, const run_length_t* const runs, const std::size_t nb_runs, const bool starting_bit_value)
            {
                int i;
                if(!starting_bit_value) //If starts by a zero, skip first run
                {
                    i = 1;
                    bit_pos += runs[0];
                }
                else
                    i = 0;

                //Write runs of 1s, skip runs of zeroes
                while(i+1 < nb_runs && bit_pos < bit_end)
                {
                    setBits(dst, bit_pos, runs[i]);

                    bit_pos += runs[i] + runs[i+1];
                    i += 2;
                }

                //Update offset is last run is a run of 0s
                if(nb_runs % 2 == starting_bit_value)
                {
                    setBits(dst, bit_pos, runs[nb_runs-1]);
                    bit_pos += runs[nb_runs-1];
                }
            }

            static void setBits(std::uint8_t* dst, std::size_t startBit, std::size_t count)
            {
                std::size_t byte = startBit / 8;
                unsigned bit = startBit & 7; //Mod 8

                // first partial byte
                if (bit)
                {
                    unsigned n = std::min<size_t>(count, 8 - bit);

                    dst[byte] |= std::uint8_t{((1u << n) - 1) << (8 - bit - n)};

                    count -= n;
                    ++byte;

                    if (!count)
                        return;
                }

                // full bytes
                std::size_t fullBytes = count / 8;
                std::memset(dst + byte, 0xFF, fullBytes);

                byte += fullBytes;
                count &= 7;

                // last partial byte
                if (count)
                    dst[byte] |= std::uint8_t{0xFF << (8 - count)};
            }

    };
}

#endif