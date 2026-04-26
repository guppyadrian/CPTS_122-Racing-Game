//
// Created by guppy on 4/23/26.
//

#include "network/Packet.hpp"

#include <array>

namespace gp::network
{
    Packet::Packet(const ByteBuffer& data)
    {
        PacketBuffer buffer(data);
        _type = buffer.read<uint8_t>();
        const auto dataLength = buffer.read<uint32_t>();
        const auto eventNameLength = buffer.read<uint16_t>();

        _eventName = buffer.readString(eventNameLength);
        _data = buffer.readBytes(dataLength);
    }

    Packet::Packet(const std::array<uint8_t, 7> &header, const ByteBuffer &body)
    {
        PacketBuffer headerBuf(header);
        _type = headerBuf.read<uint8_t>();
        const auto dataLength = headerBuf.read<uint32_t>();
        const auto eventNameLength = headerBuf.read<uint16_t>();

        PacketBuffer bodyBuf(body);
        _eventName = bodyBuf.readString(eventNameLength);
        _data = std::move(bodyBuf.readBytes(dataLength));
    }

    ByteBuffer Packet::toBytes() const
    {
        ByteBuffer buffer;
        buffer.reserve(_data.size() + 1 + _eventName.size());

        // append the header: [uint8_t type][uint32_t dataLength][uint16_t eventLength][char[] eventName][uint8_t[] data]
        buffer.push_back(_type);
        
        const uint32_t dataLength = _data.size();
        const auto dataLengthBytes = reinterpret_cast<const uint8_t*>(&dataLength);
        buffer.insert(buffer.end(), dataLengthBytes, dataLengthBytes + sizeof(uint32_t));

        const uint16_t eventNameLength = _eventName.size(); // TODO: should this be a helper function?
        const auto eventNameLengthBytes = reinterpret_cast<const uint8_t*>(&eventNameLength);
        buffer.insert(buffer.end(), eventNameLengthBytes, eventNameLengthBytes + sizeof(uint16_t));

        // append eventName and data
        buffer.insert(buffer.end(), _eventName.begin(), _eventName.end());
        buffer.insert(buffer.end(), _data.begin(), _data.end());

        return buffer;
    }
}
