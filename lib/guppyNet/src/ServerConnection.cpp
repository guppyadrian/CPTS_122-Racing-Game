//
// Created by guppy on 4/22/26.
//

#include "network/ServerConnection.hpp"

#include "asio/read.hpp"
#include "asio/write.hpp"

namespace gp::network
{
    void ServerConnection::doReadHeader()
    {
        asio::async_read(_socket, asio::buffer(_readBuffer.header(), ReadBuffer::headerMaxLength), [this](const std::error_code ec, std::size_t bytes)
        {
            if (ec)
            {
                std::cerr << "Failed to read message header!" << std::endl;
                std::cerr << ec.message() << std::endl;
                _socket.close();
                return;
            }

            doReadBody();
        });
    }

    void ServerConnection::doReadBody()
    {
        _readBuffer.body().resize(_readBuffer.getTotalLength());
        asio::async_read(_socket, asio::buffer(_readBuffer.body(), _readBuffer.getTotalLength()), [this](const std::error_code ec, std::size_t bytes)
        {
            if (ec)
            {
                std::cerr << "Failed to read message body!" << std::endl;
                std::cerr << ec.message() << std::endl;
                _socket.close();
                return;
            }

            const Packet packet = _readBuffer.collectPacket();
            
            onReceive(packet);

            doReadHeader();
        });
    }

    void ServerConnection::doWrite()
    {
        asio::async_write(_socket, asio::buffer(_writeBuffer.front().data(), _writeBuffer.front().size()), [this](const std::error_code ec, std::size_t)
        {
            if (ec)
            {
                std::cerr << "Failed to write: " << ec.message() << std::endl;
                _socket.close();
                return;
            }
            _writeBuffer.pop_front();
            if (!_writeBuffer.empty()) doWrite();
        });
    }

    void ServerConnection::onReceive(const Packet &packet)
    {
        const auto& eventName = packet.getEventName();

        if (!_listeners.contains(eventName))
        {
            std::cerr << "gp::network::NetworkClient got unhandled event type: " << eventName << std::endl;
            return;
        }

        _listeners[eventName](packet.getData());
    }

    void ServerConnection::_emit(const uint8_t type, const std::string_view eventName, const ByteBuffer &data)
    {
        const ByteBuffer bytes = Packet(type, eventName, data).toBytes();

        asio::post(_socket.get_executor(), [this, bytes = std::move(bytes)]() mutable
        {
            const bool writeInProgress = !_writeBuffer.empty();
            _writeBuffer.push_back(std::move(bytes)); // TODO: std::move?
            if (!writeInProgress) doWrite();
        });
    }

    void ServerConnection::start()
    {
        doReadHeader();
    }

    void ServerConnection::on(const std::string &eventName, const std::function<void(const ByteBuffer &)> &callback)
    {
        _listeners[eventName] = [callback](const ByteBuffer& data){ callback(data); };
    }

    void ServerConnection::on(const std::string& eventName, const std::function<void()>& callback)
    {
        _listeners[eventName] = [callback](const ByteBuffer&){ callback(); };
    }

    void ServerConnection::emit(const std::string_view eventName, const ByteBuffer &data)
    {
        _emit(0u, eventName, data);
    }
}
