//
// Created by guppy on 4/19/2026.
//

#pragma once
#include <array>

#include "Packet.hpp"
#include "network/Serialize.hpp"
#include "network/Shared.hpp"

// TODO: i notice that I feel like its extra effort dealing with header and body being separate. Might be better to just have a single vector? 
namespace gp::network
{
    class ReadBuffer
    {
    public:
        constexpr static uint8_t headerMaxLength = 7;
    private:
        std::array<uint8_t, headerMaxLength> _header{};
        uint8_t _headerLength = 0;
        ByteBuffer _body{};

    public:
        ReadBuffer() = default;

        Packet collectPacket();

        std::array<uint8_t, headerMaxLength>& header() {return _header;}
        ByteBuffer& body() {return _body;}

        [[nodiscard]] uint32_t getBodyLength() const { return ReadData<uint32_t>(_header.data() + 1); }
        [[nodiscard]] uint16_t getEventNameLength() const { return ReadData<uint16_t>(_header.data() + sizeof(uint32_t) + 1); }
        [[nodiscard]] uint8_t  getType() const { return ReadData<uint8_t>(_header.data()); }
        [[nodiscard]] uint32_t getTotalLength() const { return getBodyLength() + getEventNameLength(); }
    };
}
