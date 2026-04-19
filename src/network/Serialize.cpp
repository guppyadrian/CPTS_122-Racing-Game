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

}