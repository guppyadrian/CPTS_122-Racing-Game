//
// Created by guppy on 4/18/2026.
//

#include <iostream>

#include "network/NetworkClient.hpp"
#include <asio/connect.hpp>
#include <asio/read.hpp>
#include <asio/write.hpp>

#include "network/Packet.hpp"


namespace gp::network
{
    void NetworkClient::connect(const std::string& address, const uint16_t port)
    {
        _resolver.async_resolve(address, std::to_string(port),
            [this](const std::error_code ec, const tcp::resolver::results_type& results)
            {
                if (ec)
                {
                    std::cout << "Failed to resolve: " << ec.message() << std::endl;
                    return;
                }

                asio::async_connect(_socket, results,
                    [this](const std::error_code ec2, const tcp::endpoint& endpoint)
                    {
                        if (ec2)
                        {
                            std::cerr << "Failed to connect: " << ec2.message() << std::endl;
                            return;
                        }
                        onConnect();
                    });
            }
        );
    }

    void NetworkClient::on(const std::string& eventName, const std::function<void()>& callback)
    {
        _listeners[eventName] = [callback](const std::any&){ callback(); };
    }

    void NetworkClient::_emit(const uint8_t type, const std::string_view eventName, const ByteBuffer& data)
    {
        //const auto packet = CreatePacket(eventName, data);

        const ByteBuffer bytes = Packet(type, eventName, data).toBytes(); // TODO: technically tiny overhead to create a packet then run toBytes.

        asio::post(_io, [this, bytes]()
        {
            const bool writeInProgress = !_writeBuffer.empty();
            _writeBuffer.push_back(bytes);
            if (!writeInProgress) doWrite();
        });
    }

    void NetworkClient::onConnect()
    {
        if (_listeners.contains("connection"))
        {
            constexpr ByteBuffer vec; // TODO: this is ugly
            _listeners["connection"](vec);
        }
        doReadHeader();
    }

    void NetworkClient::doReadHeader()
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

    void NetworkClient::doReadBody()
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
            
            onReceive(packet); // TODO: take in data type? lowk just take in packet

            doReadHeader();
        });
    }

    void NetworkClient::doWrite()
    {
        asio::async_write(_socket, asio::buffer(_writeBuffer.front().data(), _writeBuffer.front().size()), [this](std::error_code ec, std::size_t bytes)
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

    void NetworkClient::onReceive(const Packet& packet)
    {
        const auto& eventName = packet.getEventName();
        
        if (!_listeners.contains(eventName))
        {
            std::cerr << "gp::network::NetworkClient got unhandled event type: " << eventName << std::endl;
            return;
        }

        _listeners[eventName](packet.getData());
    }
} // gp