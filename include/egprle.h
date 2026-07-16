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

#define isAVX2_aligned(var_name, ptr) volatile bool var_name##_AVX2_aligned = reinterpret_cast<uintptr_t>(ptr) % 32 == 0

#ifdef WRITE_RUNS
    #include <fstream>
    static std::ofstream runs_txt;
#endif

namespace EliasGammaPacker
{
    static const decode_partial_struct_t decode_partial_init_value = {
        {0}, //.mask
        {0}, //.payload1
        {0}, //.payload2
        {0}, //.selector
        {0}, //.remaining_bits
        sublane_width, //.frame_width
        {0}, //.bit_pos
        entry_point_t::init //.next_entry_point
    };

    template <typename T, std::size_t Alignment>
    class AlignedAllocator
    {
        static_assert((Alignment & (Alignment - 1)) == 0,
                    "Alignment must be a power of two");
        static_assert(Alignment >= alignof(T),
                    "Alignment must be at least alignof(T)");

    public:
        using value_type = T;

        AlignedAllocator() noexcept = default;

        template <typename U>
        constexpr AlignedAllocator(const AlignedAllocator<U, Alignment>&) noexcept {}

        [[nodiscard]]
        T* allocate(std::size_t n)
        {
            if (n > std::allocator_traits<std::allocator<T>>::max_size({}))
                throw std::bad_alloc();

            std::size_t bytes = n * sizeof(T);

            // std::aligned_alloc requires size to be a multiple of Alignment.
            std::size_t padded =
                (bytes + Alignment - 1) & ~(Alignment - 1);

            void* p = std::aligned_alloc(Alignment, padded);

            if (!p)
                throw std::bad_alloc();

            return static_cast<T*>(p);
        }

        void deallocate(T* p, std::size_t) noexcept
        {
            std::free(p);
        }

        template <typename U>
        struct rebind
        {
            using other = AlignedAllocator<U, Alignment>;
        };

        using is_always_equal = std::true_type;
    };

    template <typename T1, std::size_t A1,
            typename T2, std::size_t A2>
    constexpr bool operator==(const AlignedAllocator<T1, A1>&,
                            const AlignedAllocator<T2, A2>&) noexcept
    {
        return A1 == A2;
    }

template <typename T1, std::size_t A1,
          typename T2, std::size_t A2>
constexpr bool operator!=(const AlignedAllocator<T1, A1>& a,
                          const AlignedAllocator<T2, A2>& b) noexcept
{
    return !(a == b);
}

    class EGPRLE
    {
        private:
            //Bit-run DFA data
            //bit_run_dfa_struct_t dfa_data;
            
            //Partial decode data
            //alignas(32) decode_partial_struct_t decode_partial_data;

            //Run-length buffer
            //CircularDoubleBuffer<run_length_t, nb_runs> buffer;

            std::vector<std::uint32_t, AlignedAllocator<std::uint32_t, 32>> buffer;

            //Metadata
            metadata_struct_t meta;

        public:

            //RLE compression, can be worse than uncompressed data. But in case of RLE non-compressible data, data is stored uncompressed (TODO TO BE DONE)
            //WARNING: tard-implemented
            static std::size_t compressBound(std::size_t size)
            {
                return size + sizeof(metadata_t) + sizeof(selector_t) + 2*sizeof(payload_t);
            }

            void BitRunDFA(const std::uint8_t * const data, std::size_t length);

            static metadata_struct_t read_meta(const char* src)
            {
                metadata_t x = *reinterpret_cast<const metadata_t*>(src);
                std::uint8_t sbv = x & 1;
                std::uint8_t ir = (x >> 1) & 1;

                return metadata_struct_t{
                    x >> 2, //.out_size =
                    ir, //.is_raw =
                    sbv //.starting_bit_value =
                };
            }

            static void write_meta(char* dst, metadata_struct_t meta)
            {
                metadata_t x = (meta.out_size << 2) | ((meta.is_raw << 1) & 0b10) | (meta.starting_bit_value & 0b1);
                std::memcpy(dst, reinterpret_cast<const char*>(&x), sizeof(metadata_t));
            }

            /*void init_dfa(const char * const src)
            {
                dfa_data.pos = 0;
                dfa_data.run_length = 0;
                dfa_data.state = (*src >> 7) & 1;
                dfa_data.stop = false;
                buffer.clear();
            }*/

            void init(std::size_t size)
            {
                buffer.reserve((size+nb_runs-1)/nb_runs*nb_runs);
                buffer.clear();
            }

            std::size_t encode(char* dst, std::size_t dst_size, const char* src, std::size_t src_size)
            {
                std::uint8_t* dst_pos = reinterpret_cast<std::uint8_t*>(dst+sizeof(metadata_t));
                std::uint8_t* const dst_end = dst_pos + dst_size;

                selector_t* selector = reinterpret_cast<selector_t*>(dst_pos);
                payload_t payload1 = {0};
                payload_t payload2 = {0};

                int remaining_bits = sublane_width;
                std::size_t offset = 0;
                int frame_width;

                std::size_t buffer_offset = 0;
                const payload_t* v1 = (const payload_t*)(buffer.data());
                const payload_t* v2 = (const payload_t*)(buffer.data() + values_offset);

                #ifdef WRITE_RUNS
                    runs_txt.open("runs_encoded.txt");
                #endif

                //init_dfa(src);
                BitRunDFA(reinterpret_cast<const std::uint8_t*>(src), src_size);

                while(buffer_offset + nb_runs <= buffer.size())
                {
                    //Process 'nb_runs' integers
                    //SIMD code Group Elias Gamma here 2x AVX2 ('nb_runs'x32)
                    //Get width (log2+1) of 'nb_runs' integers
                    {
                        run_length_t merge = run_length_t{1};

                        for(int i = 0; i < nb_runs; ++i)
                            merge |= buffer[i]; //Possible to vectorize here

                        frame_width = log2<run_length_t>(merge)+1;
                    }

                    #ifdef WRITE_RUNS
                        for(int i = 0; i < nb_runs; ++i)
                            runs_txt << buffer[i]+1 << std::endl;
                    #endif

                    //Enough space
                    if(frame_width <= remaining_bits)
                    {
                        //Update selector
                        *selector |= unary<selector_t>(frame_width-1) << offset;

                        payload1 = _mm256_or_si256(_mm256_slli_epi32(*v1, offset), payload1);
                        payload2 = _mm256_or_si256(_mm256_slli_epi32(*v2, offset), payload2);

                        offset += frame_width;
                        remaining_bits -= frame_width;
                    }
                    //Need to split/move to next lane
                    else 
                    {
                        int split_frame_width = frame_width - remaining_bits;

                        if(remaining_bits != 0)
                        {
                            const payload_t split1_mask = _mm256_load_si256((const payload_t*)mask_lsb_not[split_frame_width]);
            
                            //Selector low bits go to current split (split1) - nothing to do

                            //Frame high bits go to current split (split1)
                            int shift = sublane_width - frame_width;
                            payload1 = _mm256_or_si256(_mm256_slli_epi32(_mm256_and_si256(*v1, split1_mask), shift), payload1);
                            payload2 = _mm256_or_si256(_mm256_slli_epi32(_mm256_and_si256(*v2, split1_mask), shift), payload2);
                        }

                        //Store payloads
                        dst_pos += sizeof(selector_t);
                        _mm256_storeu_si256(reinterpret_cast<payload_t*>(dst_pos), payload1);
                        dst_pos += sizeof(payload_t);
                        _mm256_storeu_si256(reinterpret_cast<payload_t*>(dst_pos), payload2);
                        dst_pos += sizeof(payload_t);

                        //Check memory out of range
                        if(dst_pos + sizeof(selector_t) + 2*sizeof(payload_t) >= dst_end)
                            throw std::runtime_error("egprle :: encode : Out of range");

                        //Update pointers (need 32-byte alignment)
                        selector = reinterpret_cast<selector_t*>(dst_pos);

                        //Selector high bits go to next split (split2)
                        *selector = unary<selector_t>(split_frame_width-1);

                        const payload_t split2_mask = _mm256_load_si256((const payload_t*)mask_lsb[split_frame_width]);
                        
                        //Frame low bits go to next split (split2)
                        payload1 = _mm256_and_si256(*v1, split2_mask);
                        payload2 = _mm256_and_si256(*v2, split2_mask);

                        //Reset offset and remaining_bits variables
                        offset = split_frame_width;
                        remaining_bits = sublane_width - split_frame_width;
                    }

                    //Move buffer offset
                    buffer_offset += nb_runs;
                    v1 = (const payload_t*)(buffer.data() + buffer_offset);
                    v2 = (const payload_t*)(buffer.data() + buffer_offset + values_offset);
                }

                if(buffer.size() % nb_runs != 0)
                {

                    //Process 'nb_runs' integers
                    //SIMD code Group Elias Gamma here 2x AVX2 ('nb_runs'x32)
                    //Get width (log2+1) of 'nb_runs' integers
                    {
                        run_length_t merge = run_length_t{1};

                        for(std::size_t i = 0; i < buffer.size() % nb_runs; ++i)
                            merge |= buffer[i]; //Possible to vectorize here

                        frame_width = log2<run_length_t>(merge)+1;
                    }


                    #ifdef WRITE_RUNS
                        for(std::size_t i = 0; i < buffer.size() % nb_runs; ++i)
                            runs_txt << buffer[i]+1 << std::endl;
                    #endif

                    //Enough space
                    if(frame_width <= remaining_bits)
                    {
                        //Update selector
                        *selector |= unary<selector_t>(frame_width-1) << offset;

                        payload1 = _mm256_or_si256(_mm256_slli_epi32(*v1, offset), payload1);
                        payload2 = _mm256_or_si256(_mm256_slli_epi32(*v2, offset), payload2);
                    }
                    //Need to split/move to next lane
                    else
                    {
                        int split_frame_width = frame_width - remaining_bits;

                        if(remaining_bits != 0)
                        {
                            const payload_t split1_mask = _mm256_load_si256((const payload_t*)mask_lsb_not[split_frame_width]);

                            //Selector low bits go to current split (split1) - nothing to do

                            //Frame high bits go to current split (split1)
                            int shift = sublane_width - frame_width;
                            payload1 = _mm256_or_si256(_mm256_slli_epi32(_mm256_and_si256(*v1, split1_mask), shift), payload1);
                            payload2 = _mm256_or_si256(_mm256_slli_epi32(_mm256_and_si256(*v2, split1_mask), shift), payload2);
                        }

                        //Store payloads
                        dst_pos += sizeof(selector_t);
                        _mm256_storeu_si256(reinterpret_cast<payload_t*>(dst_pos), payload1);
                        dst_pos += sizeof(payload_t);
                        _mm256_storeu_si256(reinterpret_cast<payload_t*>(dst_pos), payload2);
                        dst_pos += sizeof(payload_t);

                        //Check memory out of range
                        if(dst_pos + sizeof(selector_t) + 2*sizeof(payload_t) >= dst_end)
                            throw std::runtime_error("egprle :: encode : Out of range");

                        //Update pointers (need 32-byte alignment)
                        selector = reinterpret_cast<selector_t*>(dst_pos);

                        //Selector high bits go to next split (split2)
                        *selector = unary<selector_t>(split_frame_width-1);

                        const payload_t split2_mask = _mm256_load_si256((const payload_t*)mask_lsb[split_frame_width]);

                        //Frame low bits go to next split (split2)
                        payload1 = _mm256_and_si256(*v1, split2_mask);
                        payload2 = _mm256_and_si256(*v2, split2_mask);
                    }
                }

                //Store payloads
                dst_pos += sizeof(selector_t);
                _mm256_storeu_si256(reinterpret_cast<payload_t*>(dst_pos), payload1);
                dst_pos += sizeof(payload_t);
                _mm256_storeu_si256(reinterpret_cast<payload_t*>(dst_pos), payload2);
                dst_pos += sizeof(payload_t);

                #ifdef WRITE_RUNS
                    runs_txt.close();
                #endif

                return
                 + reinterpret_cast<const char*>(dst_pos)
                 - reinterpret_cast<const char*>(dst);
            }

            std::size_t decode(char* dst, std::size_t dst_size, const char* src, std::size_t src_size, const bool starting_bit_value)
            {
                const std::uint8_t* src_pos = reinterpret_cast<const std::uint8_t*>(src);

                std::uint8_t* dst_pos = reinterpret_cast<std::uint8_t*>(dst);

                const payload_t ones = _mm256_set1_epi32(1);

                std::size_t bit_pos = 0;
                const std::size_t bit_end = dst_size*8;

                selector_t selector;

                payload_t payload1;
                payload_t payload2;

                payload_t* const v1 = (payload_t*)(buffer.data());
                payload_t* const v2 = (payload_t*)(buffer.data() + values_offset);

                int frame_width;

                #ifdef WRITE_RUNS
                    runs_txt.open("runs_decoded.txt");
                #endif

                while((std::size_t)(src_pos-reinterpret_cast<const std::uint8_t*>(src)) <= src_size)
                {
                    selector = *reinterpret_cast<const selector_t*>(src_pos);
                    src_pos += sizeof(selector_t);
                    payload1 = _mm256_loadu_si256(reinterpret_cast<const payload_t*>(src_pos));
                    src_pos += sizeof(payload_t);
                    payload2 = _mm256_loadu_si256(reinterpret_cast<const payload_t*>(src_pos));
                    src_pos += sizeof(payload_t);

                    frame_width = trailing_zeroes(selector) + 1;

                    const payload_t mask = _mm256_load_si256((const payload_t*)mask_lsb[frame_width]);

                    *v1 = _mm256_add_epi32(_mm256_or_si256(_mm256_slli_epi32(*v1, frame_width), _mm256_and_si256(payload1, mask)), ones);
                    *v2 = _mm256_add_epi32(_mm256_or_si256(_mm256_slli_epi32(*v2, frame_width), _mm256_and_si256(payload2, mask)), ones);

                    #ifdef WRITE_RUNS
                        for(int i = 0; i < nb_runs; ++i)
                            runs_txt << values[i] << std::endl;
                    #endif

                    payload1 = _mm256_srli_epi32(payload1, frame_width);
                    payload2 = _mm256_srli_epi32(payload2, frame_width);

                    selector >>= frame_width;
                    
                    while(selector != 0)
                    {
                        frame_width = trailing_zeroes(selector) + 1;

                        const payload_t mask = _mm256_load_si256((const payload_t*)mask_lsb[frame_width]);

                        *v1 = _mm256_add_epi32(_mm256_and_si256(payload1, mask), ones);
                        *v2 = _mm256_add_epi32(_mm256_and_si256(payload2, mask), ones);

                        #ifdef WRITE_RUNS
                            for(int i = 0; i < nb_runs; ++i)
                                runs_txt << values[i] << std::endl;
                        #endif
                        
                        payload1 = _mm256_srli_epi32(payload1, frame_width);
                        payload2 = _mm256_srli_epi32(payload2, frame_width);

                        selector >>= frame_width;
                    }
                    
                    //Handle overlap (even if no overlap, "if" would slow process)
                    //Extract values from payloads (last frame)
                    *v1 = payload1;
                    *v2 = payload2;
                }

                if(starting_bit_value)
                    decode_bit_runs_s1(dst_pos, bit_pos, bit_end, buffer.data(), buffer.size());
                else
                    decode_bit_runs_s0(dst_pos, bit_pos, bit_end, buffer.data(), buffer.size());

                #ifdef WRITE_RUNS
                    runs_txt.close();
                #endif

                return bit_pos / 8; //Number of written bytes
            }

            //Not implemented yet
            std::size_t decode_partial(char* dst, std::size_t dst_size, const char* src, std::size_t src_size, std::size_t decode_until_size);/*
            {
                decode_partial_struct_t& d = decode_partial_data;

                alignas(32) run_length_t values[nb_runs] = {0};
                payload_t* const v1 = (payload_t*)(values);
                payload_t* const v2 = (payload_t*)(values+values_offset);

                int constexpr read_size = sizeof(selector_t) + 2*sizeof(payload_t);

                const std::uint8_t* src_pos = reinterpret_cast<const std::uint8_t*>(src+sizeof(metadata_t));
                const std::uint8_t* const src_end = reinterpret_cast<const std::uint8_t*>(src) + src_size;

                std::uint8_t* dst_pos = reinterpret_cast<std::uint8_t*>(dst);

                const payload_t ones = _mm256_set1_epi32(1);

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
                        throw std::runtime_error("egprle :: decode_partial : unexpected entry point");
                }
                
                while(src_pos + read_size <= src_end)
                {
                    d.remaining_bits = sublane_width;

                    d.selector = *reinterpret_cast<const selector_t*>(src_pos);

                    d.payload1 = _mm256_loadu_si256(reinterpret_cast<const payload_t*>(src_pos + sizeof(selector_t)));
                    d.payload2 = _mm256_loadu_si256(reinterpret_cast<const payload_t*>(src_pos + sizeof(selector_t) + sizeof(payload_t)));

                    d.frame_width = trailing_zeroes(d.selector) + 1;
                    d.mask = _mm256_load_si256((const payload_t*)mask_lsb[d.frame_width]);

                    *v1 = _mm256_add_epi32(_mm256_or_si256(_mm256_slli_epi32(*v1, d.frame_width), _mm256_and_si256(d.payload1, d.mask)), ones);
                    *v2 = _mm256_add_epi32(_mm256_or_si256(_mm256_slli_epi32(*v2, d.frame_width), _mm256_and_si256(d.payload2, d.mask)), ones);

                    d.next_entry_point = entry_point_t::begin;
                    decode_bit_runs(dst_pos, d.bit_pos, bit_end, values, nb_runs, meta.starting_bit_value);
                egp_begin:
                    d.payload1 = _mm256_srli_epi32(d.payload1, d.frame_width);
                    d.payload2 = _mm256_srli_epi32(d.payload2, d.frame_width);

                    d.selector >>= d.frame_width;
                    d.remaining_bits -= d.frame_width;
                    
                    while(d.selector != 0)
                    {
                        d.frame_width = trailing_zeroes(d.selector) + 1;
                        d.mask = _mm256_load_si256((const payload_t*)mask_lsb[d.frame_width]);

                        *v1 = _mm256_add_epi32(_mm256_and_si256(d.payload1, d.mask), ones);
                        *v2 = _mm256_add_epi32(_mm256_and_si256(d.payload2, d.mask), ones);
                        d.next_entry_point = entry_point_t::inner;
                        decode_bit_runs(dst_pos, d.bit_pos, bit_end, values, nb_runs, meta.starting_bit_value);
                egp_inner:
                        d.payload1 = _mm256_srli_epi32(d.payload1, d.frame_width);
                        d.payload2 = _mm256_srli_epi32(d.payload2, d.frame_width);

                        d.selector >>= d.frame_width;
                        d.remaining_bits -= d.frame_width; //Note: can't be negative as sum of frame_width is less than or equal to 32
                    }
                    
                    //Handle overlap (even if no overlap, "if" would slow process)
                    //Extract values from payloads (last frame)
                    *v1 = d.payload1;
                    *v2 = d.payload2;
                    
                    //Move cursor
                    src_pos += read_size;
                }
            }*/

            //Not implemented yet
            std::size_t decode_partial_backward(char* dst, std::size_t dst_size, const char* src, std::size_t src_size, std::size_t decode_until_size);

            //Encode 'nb_runs' runs of bits into 'dst', return current byte position
            static void decode_bit_runs_s0(std::uint8_t* const dst, std::size_t& bit_pos, const std::size_t bit_end, const run_length_t* const runs, const std::size_t count)
            {
                std::size_t i = 1;

                bit_pos += runs[0];

                //Write runs of 1s, skip runs of zeroes
                while(i+1 < nb_runs)
                {
                    if(bit_pos + runs[i] > bit_end)
                        return;

                    setBits(dst, bit_pos, runs[i]);

                    bit_pos += runs[i] + runs[i+1];
                    i += 2;
                }

                //Update offset is last run is a run of 0s
                if(bit_pos + runs[nb_runs-1] < bit_end)
                {
                    setBits(dst, bit_pos, runs[nb_runs-1]);
                    bit_pos += runs[nb_runs-1];
                }
            }

            //Encode 'nb_runs' runs of bits into 'dst', return current byte position
            static void decode_bit_runs_s1(std::uint8_t* const dst, std::size_t& bit_pos, const std::size_t bit_end, const run_length_t* const runs, const std::size_t count)
            {
                std::size_t i = 0;

                //Write runs of 1s, skip runs of zeroes
                while(i+1 < nb_runs)
                {
                    if(bit_pos + runs[i] > bit_end)
                        return;

                    setBits(dst, bit_pos, runs[i]);

                    bit_pos += runs[i] + runs[i+1];
                    i += 2;

                }
            }

            //Assumes that destination is zeroed.
            static void setBits(std::uint8_t* dst, std::size_t startBit, std::size_t count)
            {
                std::size_t endBit    = startBit + count - 1; // last bit to set (inclusive)
                std::size_t startByte = startBit  >> 3;       // startBit / 8
                std::size_t endByte   = endBit    >> 3;        // endBit   / 8

                if (startByte == endByte)
                {
                    // ── All bits live in a single byte ──────────────────────────────────
                    // Build a mask for bits [startBit%8 .. endBit%8].
                    // e.g. startBit=2, count=4  →  0b00111100
                    std::uint8_t mask = static_cast<std::uint8_t>(
                        (0xFFu >> (startBit & 7u)) &               // clear bits above start
                        (0xFFu << (7u - (endBit & 7u)))            // clear bits below end
                    );
                    dst[startByte] |= mask;
                    return;
                }

                // ── Partial first byte ───────────────────────────────────────────────────
                // Bits from (startBit%8) to 7 must be set.
                // 0xFFu >> k  gives a mask with the top (8-k) bits set (k = startBit & 7).
                dst[startByte] |= static_cast<std::uint8_t>(0xFFu >> (startBit & 7u));

                // ── Full middle bytes ─────────────────────────────────────────────────────
                // dst is guaranteed to be all-zero, so |= 0xFF == = 0xFF == memset.
                if (endByte - startByte > 1)
                    std::memset(dst + startByte + 1, 0xFF, endByte - startByte - 1);

                // ── Partial last byte ────────────────────────────────────────────────────
                // Bits from 0 to (endBit%8) must be set.
                // 0xFFu << (7 - k)  gives a mask with the bottom (k+1) bits set (k = endBit & 7).
                dst[endByte] |= static_cast<std::uint8_t>(0xFFu << (7u - (endBit & 7u)));
            }
    };
}

#endif