#ifndef EGPRLE_H
#define EGPRLE_H
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <vector>
#include <cstdlib>

typedef std::uint32_t run_length_type;

template<typename N, std::size_t size>
class DoubleCircularBuffer
{
    private:
        N* buffer;
        std::size_t offset;
        std::size_t get_offset;
    public:
        DoubleCircularBuffer()
        {
            this->buffer = new N[size*2];
            clear();
        }

        DoubleCircularBuffer(const DoubleCircularBuffer<N,size>& other) noexcept
        {
            if(this != &other)
            {
                std::memcpy(this->buffer, other.buffer, sizeof(other.buffer));
                this->offset = other.offset;
                this->get_offset = other.get_offset;
            }
        }

        DoubleCircularBuffer(DoubleCircularBuffer<N,size>&& other) noexcept
            : buffer(other.buffer), offset(other.offset), get_offset(other.get_offset)
        {
            other.buffer = nullptr;
        }

        DoubleCircularBuffer<N, size>& operator=(DoubleCircularBuffer<N, size>&& other) noexcept {
            if (this != &other) 
            {
                delete[] buffer;

                this->buffer = other.buffer;
                this->offset = other.offset;
                this->get_offset = other.get_offset;
                std::memcpy(this->buffer, other.buffer, sizeof(other.buffer));

                other.buffer = nullptr;
            }

            return *this;
        }

        DoubleCircularBuffer<N, size>& operator=(const DoubleCircularBuffer<N, size>& other)
        {
            if(this != &other)
            {
                std::memcpy(this->buffer, other.buffer, sizeof(other.buffer));
                this->offset = other.offset;
                this->get_offset = other.get_offset;
            }

            return *this;
        }

        void clear()
        {
            this->offset = 0;
            this->get_offset = 0;
        }

        virtual ~DoubleCircularBuffer()
        {
            if(buffer != nullptr)
                delete[] buffer;
            buffer = nullptr;
        }
    
        void push(const N& value)
        {
            buffer[offset++] = value;
            offset %= size;
        }

        const N* ptr() const
        {
            return buffer + get_offset;
        }

        std::size_t size() const
        {
            return size;
        }

        void cycle()
        {
            get_offset ^= size;
        }
};

namespace EliasGammaPacker
{
    class EGPRLE
    {
        private:
            std::size_t dfa_pos;
            run_length_type dfa_run_length;
            std::uint8_t dfa_state;
            DoubleCircularBuffer<run_length_type, 16> buffer;
        public:

            //RLE compression, can be worse than uncompressed data. But in case of RLE non-compressible data, data is stored uncompressed (TODO TO BE DONE)
            //WARNING: tard-implemented
            static std::size_t compressBound(std::size_t data_size)
            {
                return 2*data_size;
            }

            void BitRunDFA(const std::uint8_t * const data, std::size_t length);

            void std::size_t inline encode(char* dst, std::size_t dst_size, const char* src, std::size_t src_size)
            {
                dfa_pos = 0;
                dfa_run_length = 0;
                buffer.clear();

                //Get integers to compress (TODO: yield them (coroutine ??), for avoiding having all integers in memory)
                BitRunDFA(reinterpret_cast<const std::uint8_t*>(src), src_size);

                integers.clear();
                return compressor.encode(dst, dst_size, integers.data(), integers.size());
                /*        throw std::runtime_error("ERROR EGPRLE::encode: destination memory is not big enough.");
                    else
                        std::memcpy(dst, src, src_size);*/
            }

            static std::size_t forceinline decode(char* dst, std::size_t dst_size, const char* src, std::size_t src_size)
            {
                std::size_t bit_pos = 0;
                std::size_t run_length = 0;
                
                integers.clear();
                compressor.decode(integers.data(), src, src_size);

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
                }
            }


    };
}

#endif