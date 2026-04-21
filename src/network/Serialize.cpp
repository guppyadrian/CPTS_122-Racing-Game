//
// Created by guppy on 4/18/2026.
//

#include "network/Serialize.hpp"

namespace gp::network
{
    std::vector<uint8_t> Serialize(std::string_view string)
    {
        return std::vector<uint8_t>(string.begin(), string.end());
    }

    std::vector<uint8_t> Serialize(const json& json)
    {
        return Serialize(static_cast<std::string_view>(json.dump()));
    }

    std::vector<uint8_t> CreatePacket(std::string_view eventName, const std::vector<uint8_t>& data)
    {
        if (eventName.size() > UINT16_MAX) throw std::invalid_argument("Failed to create NetworkPacket because event name was too long: " + std::to_string(eventName.size()));

        // packet format: [uint32_t bytesSize][uint16_t eventNameSize][char* eventName][uint8_t bytes]
        std::vector<uint8_t> packet;
        packet.reserve(data.size() + sizeof(uint32_t) + sizeof(uint16_t) + eventName.size()); // should be what I need to reserve right?

        // append sizes
        packet.push_back(2u); // data type TODO: make enum for this?
        AppendAsBytes(packet, static_cast<uint32_t>(data.size()));
        AppendAsBytes(packet, static_cast<uint16_t>(eventName.size())); // TODO: checking to make sure this doesn't overflow

        packet.insert(packet.end(), eventName.begin(), eventName.end());
        packet.insert(packet.end(), data.begin(), data.end());

        return packet;
    }

    NetworkPacket ParsePacket(const std::vector<uint8_t>& data)
    {
        int offset = 0;

        const uint8_t type = data[0];
        offset += 1;

        const auto dataLength = ReadData<uint32_t>(data, offset);
        const auto eventNameLength = ReadData<uint16_t>(data, offset);

        return ParsePacket(type, eventNameLength, dataLength, std::vector<uint8_t>(data.begin() + offset, data.begin() + offset + eventNameLength + dataLength));
    }

    NetworkPacket ParsePacket(const uint8_t type, const int eventNameLength, const int dataLength, const std::vector<uint8_t>& body)
    {
        NetworkPacket packet;
        packet.type = type;

        packet.eventName = std::string(reinterpret_cast<const char*>(body.data()), eventNameLength);
        packet.data.insert(packet.data.end(), body.data() + eventNameLength, body.data() + eventNameLength + dataLength);

        return packet;
    }
}
