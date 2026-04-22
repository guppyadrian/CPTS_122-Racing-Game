//
// Created by guppy on 4/18/2026.
//

#pragma once

#include <deque>

#include "network/ReadBuffer.hpp"
#include "network/NetworkManager.hpp"
#include "asio/ip/tcp.hpp"
#include "network/Serialize.hpp"

using asio::ip::tcp;

template <typename F, typename T>
concept EventCallback = std::invocable<F, T> && std::same_as<std::invoke_result_t<F, T>, void>; // concept slop
// ^ got this from chatGPT. I wanted to use std::function, but it doesn't work well with templates :(

namespace gp::network
{
    class NetworkClient
    {
    private:
        asio::io_context& _io;
        tcp::resolver _resolver;
        tcp::socket _socket;

        ReadBuffer _readBuffer;
        std::deque<std::vector<uint8_t>> _writeBuffer;

        std::unordered_map<std::string, std::function<void(const std::vector<uint8_t>&)>> _listeners;
    public:
        NetworkClient() : _io(NetworkManager::io()), _resolver(_io), _socket(_io) {}
        explicit NetworkClient(asio::io_context& io) : _io(io), _resolver(_io), _socket(_io) {} // alternative constructor if you want to manually pass the io

        void connect(const std::string& address, uint16_t port);

        template<NetworkData T>
        void emit(std::string_view eventName, const T& data);

        template<Serializable T, EventCallback<T> F>
        void on(const std::string& eventName, F callback);
        void on(const std::string& eventName, const std::function<void()>& callback);

        [[nodiscard]] bool connected() const { return _socket.is_open(); }

    private:
        void send(std::string_view eventName, const std::vector<uint8_t>& data);
        void _onConnect();
        void _onWrite();

        void _onReceive(const std::string& eventName, const std::vector<uint8_t>& data);

        void doReadHeader();
        void doReadBody(); // TODO: start renaming stuff to Body instead of Data
    };

    template <NetworkData T>
    void NetworkClient::emit(std::string_view eventName, const T& data)
    {
        if constexpr (!Serializable<T>) // means that this is already a byte vector  TODO: maybe be more verbose about this?
        {
            send(eventName, data);
            return;
        }
        send(eventName, Serialize(data));
    }

    template<Serializable T, EventCallback<T> F>
    void NetworkClient::on(const std::string &eventName, const F callback)
    {
        _listeners[eventName] = [callback](const std::vector<uint8_t>& data)
        {
            callback(Deserialize<T>(data));
        };
    }
} // gp