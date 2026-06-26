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
        std::size_t nb_pushed_value = {0};
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
            : buffer(other.buffer), offset(other.offset), get_offset(other.get_offset) {}

        CircularDoubleBuffer<N, _size>& operator=(CircularDoubleBuffer<N, _size>&& other) noexcept {
            if (this != &other) 
            {
                this->buffer = other.buffer;
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
            return *(ptr()+i);
        }

        void clear()
        {
            offset = 0;
            get_offset = 0;
            nb_pushed_value = 0;
            std::memset(buffer, 0, 2*sizeof(N)*_size);
        }

        virtual ~CircularDoubleBuffer()
        {
            clear();
        }
    
        void push(const N& value)
        {
            buffer[offset++] = value;
            offset %= _size*2;
        }

        N* ptr()
        {
            return buffer + get_offset;
        }

        const N* ptr() const
        {
            return buffer + get_offset;
        }

        //Warning: May return "_size" if empty.
        std::size_t constexpr size() const
        {
            return _size;
        }

        void cycle()
        {
            get_offset ^= _size;
        }
};

#endif