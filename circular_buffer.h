#ifndef EGP_CIRCULAR_BUFFER_H
#define EGP_CIRCULAR_BUFFER_H

#include <cstdint>

template<typename N, std::size_t _size>
class CircularDoubleBuffer
{
    private:
        alignas(32) N buffer[_size*2] = {0};
        std::size_t offset = {0};
        std::size_t get_offset = {0};
    public:
        CircularDoubleBuffer() noexcept {}

        CircularDoubleBuffer(const CircularDoubleBuffer<N,_size>& other) noexcept
        {
            if(this != &other)
            {
                std::memcpy(this->buffer, other.buffer, sizeof(other.buffer));
                this->offset = other.offset;
                this->get_offset = other.get_offset;
            }
        }

        CircularDoubleBuffer(CircularDoubleBuffer<N, _size>&& other) noexcept
            : offset(other.offset), get_offset(other.get_offset)
        {
            std::memcpy(buffer, other.buffer, sizeof(buffer));
        }

        CircularDoubleBuffer<N, _size>& operator=(CircularDoubleBuffer<N, _size>&& other) noexcept
        {
            if (this != &other)
            {
                this->offset = other.offset;
                this->get_offset = other.get_offset;
                std::memcpy(this->buffer, other.buffer, sizeof(other.buffer));
            }

            return *this;
        }

        CircularDoubleBuffer<N, _size>& operator=(const CircularDoubleBuffer<N, _size>& other)
        {
            if(this != &other)
            {
                std::memcpy(this->buffer, other.buffer, sizeof(other.buffer));
                this->offset = other.offset;
                this->get_offset = other.get_offset;
            }

            return *this;
        }

        const N& operator[](std::size_t i) const
        {
            //return *(ptr()+i)
            return buffer[get_offset + i];
        }

        constexpr void clear() noexcept
        {
            offset = 0;
            get_offset = 0;
            std::memset(buffer, 0, 2*sizeof(N)*_size);
        }

        ~CircularDoubleBuffer() noexcept
        {
            clear();
        }
    
        constexpr void push(const N& value) noexcept
        {
            buffer[offset++] = value;
            offset %= _size*2; //should be optimized by compiler as "offset &= _size*2-1;"
        }

        constexpr N* ptr() noexcept
        {
            return buffer + get_offset;
        }

        constexpr const N* ptr() const noexcept
        {
            return buffer + get_offset;
        }

        //Warning: May return "_size" if empty.
        constexpr std::size_t size() const noexcept
        {
            return _size;
        }

        constexpr void cycle() noexcept
        {
            get_offset ^= _size;
        }
};

#endif