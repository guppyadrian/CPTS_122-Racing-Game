//
// Created by guppy on 4/18/2026.
//

#pragma once

#include <deque>

#include "network/NetworkManager.hpp"
#include "asio/ip/tcp.hpp"
#include "Serialize.hpp"

using asio::ip::tcp;

namespace gp::network
{
    class NetworkClient
    {
    private:
        asio::io_context& _io;
        tcp::resolver _resolver;
        tcp::socket _socket;

        std::array<char, 1024> _readBuffer{}; // getting stuff from server... Maybe 1024 is too small? TODO: can this be a vector safely?
        std::deque<std::vector<uint8_t>> _writeBuffer;
    public:
        NetworkClient() : _io(NetworkManager::io()), _resolver(_io), _socket(_io) {}
        explicit NetworkClient(asio::io_context& io) : _io(io), _resolver(_io), _socket(_io) {} // alternative constructor if you want to manually pass the io

        void connect(const std::string& address, uint16_t port);

        template<NetworkPacket T>
        void emit(std::string_view eventName, const T& data);

        [[nodiscard]] bool connected() const { return _socket.is_open(); }

    private:
        void send(std::string_view eventName, std::vector<uint8_t> data);
        void _onConnect();
        void _onWrite();
    };

    template <NetworkPacket T>
    void NetworkClient::emit(std::string_view eventName, const T& data)
    {
        if constexpr (!Serializable<T>) // means that this is already a byte vector  TODO: maybe be more verbose about this?
        {
            send(eventName, data);
            return;
        }
        send(eventName, Serialize(data));
    }
} // gp