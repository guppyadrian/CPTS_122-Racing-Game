//
// Created by guppy on 4/18/2026.
//

#pragma once

#include <deque>

#include "network/ReadBuffer.hpp"
#include "network/NetworkManager.hpp"
#include "asio/ip/tcp.hpp"
#include "network/Serialize.hpp"
#include "network/Shared.hpp"

using asio::ip::tcp;

namespace gp::network
{
    class NetworkClient
    {
    private:
        asio::io_context& _io;
        tcp::resolver _resolver;
        tcp::socket _socket;

        ReadBuffer _readBuffer;
        std::deque<ByteBuffer> _writeBuffer;

        std::unordered_map<std::string, std::function<void(const ByteBuffer&)>> _listeners;
    public:
        NetworkClient() : _io(NetworkManager::io()), _resolver(_io), _socket(_io) {}
        explicit NetworkClient(asio::io_context& io) : _io(io), _resolver(_io), _socket(_io) {} // alternative constructor if you want to manually pass the io

        // connect to a TCP server using address and port
        void connect(const std::string& address, uint16_t port);

        // Emits an event to the server
        template<Serializable T>
        void emit(std::string_view eventName, const T& data);
        
        // Emits an event to the server, sends raw bytes
        void emit(std::string_view eventName, const ByteBuffer& data);

        template<Serializable T, EventCallback<T> F>
        void on(const std::string& eventName, F callback);
        void on(const std::string& eventName, const std::function<void()>& callback);

        [[nodiscard]] bool connected() const { return _socket.is_open(); }

    private:
        // called by emit(), adds event to write queue
        void _emit(uint8_t type, std::string_view eventName, const ByteBuffer& data);

        // called when socket connects to server
        void onConnect();
        // called when a full packet is received from the server
        void onReceive(const Packet& packet);

        // this 'do' stuff is ran async
        void doWrite();
        void doReadHeader();
        void doReadBody(); // TODO: start renaming stuff to Body instead of Data
    };

    template <Serializable T>
    void NetworkClient::emit(const std::string_view eventName, const T& data)
    {
        _emit(2u, eventName, Serialize(data)); // TODO: have a SerializeType() thing to find which type this is or something
    }

    template<Serializable T, EventCallback<T> F>
    void NetworkClient::on(const std::string &eventName, const F callback)
    {
        _listeners[eventName] = [callback](const ByteBuffer& data)
        {
            callback(Deserialize<T>(data));
        };
    }
} // gp