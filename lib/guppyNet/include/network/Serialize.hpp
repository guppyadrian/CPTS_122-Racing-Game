//
// Created by guppy on 4/18/2026.
//

#pragma once
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <json/json.hpp>

#include "Shared.hpp"

using nlohmann::json;

namespace gp::network
{
    /*
     * 0: bytes
     * 1: json
     * 2: string
     *
     */

    ByteBuffer Serialize(const std::string& string);
    ByteBuffer Serialize(std::string_view string);
    template <std::size_t N>
    ByteBuffer Serialize(const char (&string)[N]);

    ByteBuffer Serialize(const json& json);

    template <typename T>
    requires std::is_trivially_copyable_v<T>
    ByteBuffer Serialize(const T& value);

    // Emitting should only allow values which have a Serialize() function for them!
    template <typename T>
    concept Serializable = requires(const T& t) { Serialize(t); };

    template <Serializable T>
    T Deserialize(const ByteBuffer& data);

    template <>
    inline std::string Deserialize<std::string>(const ByteBuffer &data)
    {
        std::string string(data.begin(), data.end());

        return string;
    }

    template <typename T>
    requires std::is_trivially_copyable_v<T>
    T Deserialize(const ByteBuffer& data)
    {
        T value;
        std::memcpy(&value, data.begin(), sizeof(T));
        return value;
    }

    template<std::size_t N>
    ByteBuffer Serialize(const char(&string)[N])
    {
        return ByteBuffer(string, string + N - 1);
    }

    template<typename T> requires std::is_trivially_copyable_v<T>
    ByteBuffer Serialize(const T& value)
    {
        const auto valueBytes = reinterpret_cast<const uint8_t*>(&value);
        return ByteBuffer(valueBytes, valueBytes + sizeof(value));
    }

    // TODO: maybe deprecated? only used in ReadBuffer but function might be unused
    template <typename T>
    T ReadData(const ByteBuffer& data, int& offset)
    {
        if (data.size() < offset + sizeof(T)) throw std::runtime_error("Tried reading data bigger than buffer!");
        T value = 0;
        std::memcpy(&value, data.data() + offset, sizeof(T));
        offset += sizeof(T);
        return value;
    }

    // Note: this can cause an overflow! Please be careful you are reading something smaller than the buffer
    template <typename T>
    T ReadData(const uint8_t* data, const int offset = 0) // TODO: make readonly and non-readonly versions lowk
    {
        T value = 0;
        std::memcpy(&value, data + offset, sizeof(T));
        return value;
    }
}