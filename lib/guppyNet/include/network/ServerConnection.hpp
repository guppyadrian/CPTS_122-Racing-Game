//
// Created by guppy on 4/22/26.
//

#pragma once
#include <deque>

#include "ReadBuffer.hpp"
#include "asio/ip/tcp.hpp"

using asio::ip::tcp;

namespace gp::network
{
    class ServerConnection
    {
    private:
        tcp::socket _socket;
        ReadBuffer _readBuffer;
        std::deque<ByteBuffer> _writeBuffer;
        std::unordered_map<std::string, std::function<void(const ByteBuffer&)>> _listeners;
        std::function<void()> _onClose;
    public:
        explicit ServerConnection(tcp::socket socket) : _socket(std::move(socket)) {}
        // Starts listening :)... TODO: wait shouldn't this be called in constructor? so user can't call start()!
        void start();

        void on(const std::string& eventName, const std::function<void(const ByteBuffer&)>& callback);
        template <Serializable T, EventCallback<T> F>
        void on(const std::string& eventName, const F& callback);
        void on(const std::string& eventName, const std::function<void()>& callback);
        template <Serializable T>
        void emit(std::string_view eventName, const T& data);
        void emit(std::string_view eventName, const ByteBuffer& data);
        void close() { _onClose(); }
    private:
        void doReadHeader();
        void doReadBody();
        void doWrite();

        void onReceive(const Packet& packet);

        void _emit(uint8_t type, std::string_view eventName, const ByteBuffer& data);
    };

    template<Serializable T, EventCallback<T> F>
    void ServerConnection::on(const std::string &eventName, const F &callback)
    {
        _listeners[eventName] = [callback](const ByteBuffer& data) // TODO: std::move instead of pass by ref?
        {                                                                //       is it possible that pass by ref breaks stuff?
            callback(Deserialize<T>(data));
        };
    }

    template<Serializable T>
    void ServerConnection::emit(const std::string_view eventName, const T &data)
    {
        _emit(2u, eventName, Serialize(data));
    }
}