//
// Created by guppy on 4/19/2026.
//

#pragma once
#include <array>
#include <vector>

#include "Serialize.hpp"

// TODO: work on this more later
namespace gp::network
{
    class ReadBuffer
    {
    public:
        constexpr static uint8_t headerMaxLength = 7;
    private:
        std::array<uint8_t, headerMaxLength> _header{};
        uint8_t _headerLength = 0;
        std::vector<uint8_t> _body{};

    public:
        ReadBuffer() = default;

        // returns true if a packet is completed
        bool readData(const uint8_t* start, uint32_t length);

        NetworkPacket collectPacket();
        bool hasReadyPacket() {return true;} // TODO: implement

        std::array<uint8_t, headerMaxLength>& header() {return _header;}
        std::vector<uint8_t>& body() {return _body;}

        [[nodiscard]] uint32_t getBodyLength() const { return ReadData<uint32_t>(_header.data() + 1); }
        [[nodiscard]] uint16_t getEventNameLength() const { return ReadData<uint16_t>(_header.data() + sizeof(uint32_t) + 1); }
        [[nodiscard]] uint8_t  getType() const { return ReadData<uint8_t>(_header.data()); }
        [[nodiscard]] uint32_t getTotalLength() const { return getBodyLength() + getEventNameLength(); }
    };
}
