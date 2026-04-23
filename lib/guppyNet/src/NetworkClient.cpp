//
// Created by guppy on 4/18/2026.
//

#include <iostream>

#include "asio.hpp"

#include "network/NetworkClient.hpp"



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
                        _onConnect();
                    });
            }
        );
    }

    void NetworkClient::on(const std::string &eventName, const std::function<void()>& callback)
    {
        _listeners[eventName] = [callback](const std::any& data){ callback(); };
    }

    void NetworkClient::send(const std::string_view eventName, const std::vector<uint8_t>& data)
    {
        const auto packet = CreatePacket(eventName, data);

        asio::post(_io, [this, packet]()
        {
            const bool writeInProgress = !_writeBuffer.empty();
            _writeBuffer.push_back(packet);
            if (!writeInProgress) _onWrite();
        });
    }

    void NetworkClient::_onConnect()
    {
        if (_listeners.contains("connection"))
        {
            std::vector<uint8_t> vec; // TODO: this is ugly
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

            const NetworkPacket packet = _readBuffer.collectPacket();
            
            _onReceive(packet.eventName, packet.data); // TODO: take in data type? lowk just take in packet

            doReadHeader();
        });
    }

    void NetworkClient::_onWrite() // TODO: maybe call this doWrite also while ur at it also change _onConnect to a better name
    {
        const auto t = _writeBuffer.front();
        asio::async_write(_socket, asio::buffer(_writeBuffer.front().data(), _writeBuffer.front().size()), [this](std::error_code ec, std::size_t bytes)
        {
            if (ec)
            {
                std::cerr << "Failed to write: " << ec.message() << std::endl;
                _socket.close();
                return;
            }
            _writeBuffer.pop_front();
            if (!_writeBuffer.empty()) _onWrite();
        });
    }

    void NetworkClient::_onReceive(const std::string& eventName, const std::vector<uint8_t>& data)
    {
        if (!_listeners.contains(eventName.data()))
        {
            std::cerr << "gp::network::NetworkClient got unhandled event type: " << eventName << std::endl;
            return;
        }

        _listeners[eventName](data);
    }
} // gp