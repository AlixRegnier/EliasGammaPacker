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
        CircularDoubleBuffer()
        {

        }

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
            : buffer(other.buffer), offset(other.offset), get_offset(other.get_offset)
        {

        }

        CircularDoubleBuffer<N, _size>& operator=(CircularDoubleBuffer<N, _size>&& other) noexcept {
            if (this != &other) 
            {
                delete[] buffer;

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
            this->offset = 0;
            this->get_offset = 0;
            std::memset(buffer, 0, sizeof(buffer));
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

        std::size_t buffer_size() const
        {
            return _size;
        }

        std::size_t size() const
        {
            return (offset % _size) + 1;
        }

        void cycle()
        {
            get_offset ^= _size;
        }
};

#endif