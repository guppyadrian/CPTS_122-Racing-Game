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
