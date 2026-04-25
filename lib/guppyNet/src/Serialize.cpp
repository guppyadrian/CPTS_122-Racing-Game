//
// Created by guppy on 4/18/2026.
//

#include "network/Serialize.hpp"

namespace gp::network
{
    ByteBuffer Serialize(const std::string& string)
    {
        return ByteBuffer(string.begin(), string.end());
    }

    ByteBuffer Serialize(const std::string_view string)
    {
        return ByteBuffer(string.begin(), string.end());
    }

    ByteBuffer Serialize(const json& json)
    {
        return Serialize(json.dump());
    }
}
