//
// Created by guppy on 4/23/26.
//

#pragma once
#include <array>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>

#include "Shared.hpp"

namespace gp::network
{
    class Packet
    {
    private:
        uint8_t _type;
        std::string _eventName;
        ByteBuffer _data;

        class PacketBuffer
        {
        private:
            int _offset = 0;
            const uint8_t* _buffer;
            uint32_t _size;
        public:
            explicit PacketBuffer(const ByteBuffer& buffer) : _buffer(buffer.data()), _size(buffer.size()) {}
            template <size_t N>
            explicit PacketBuffer(const std::array<uint8_t, N>& buffer) : _buffer(buffer.data()), _size(N) {}
            template <typename T>
            T read()
            {
                if (_size < _offset + sizeof(T)) throw std::runtime_error("Tried reading data bigger than buffer!");
                T value = 0;
                std::memcpy(&value, _buffer + _offset, sizeof(T));
                _offset += sizeof(T);
                return value;
            }
            std::string readString(const int length)
            {
                if (_size < _offset + length) throw std::runtime_error("Tried reading string bigger than buffer!");
                std::string str(reinterpret_cast<const char *>(_buffer + _offset), length);
                _offset += length;
                return str;
            }
            ByteBuffer readBytes(const int length)
            {
                if (_size < _offset + length) throw std::runtime_error("Tried reading bytes bigger than buffer!");
                std::vector buffer(_buffer + _offset, _buffer + _offset + length);
                _offset += length;
                return buffer;
            }
        };
        
    public:
        Packet(const uint8_t type, const std::string_view eventName, const ByteBuffer& data) : _type(type), _eventName(eventName), _data(data) {}
        explicit Packet(const ByteBuffer& data); // TODO: unused rn!
        Packet(const std::array<uint8_t, 7>& header, const ByteBuffer& body);

        ByteBuffer toBytes() const;

        uint8_t getType() const { return _type; }
        const std::string& getEventName() const { return _eventName; }
        const ByteBuffer& getData() const { return _data; }
    };
}
