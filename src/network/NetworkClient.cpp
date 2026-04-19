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
        std::cout << "Connecting to " << address << ":" << port << std::endl;
        _resolver.async_resolve(address, std::to_string(port),
            [this](const std::error_code ec, const tcp::resolver::results_type& results)
            {
                std::cout << "resolved!" << std::endl;
                if (ec)
                {
                    std::cout << "Failed to resolve: " << ec.message() << std::endl;
                    return;
                }

                asio::async_connect(_socket, results,
                    [this](const std::error_code ec, const tcp::endpoint& endpoint)
                    {
                        if (ec)
                        {
                            std::cout << "Failed to connect: " << ec.message() << std::endl;
                            return;
                        }

                        std::cout << "Connected" << std::endl;
                        _onConnect();
                    });
            }
        );
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
        doReadHeader();
    }

    void NetworkClient::doReadHeader()
    {
        asio::async_read(_socket, asio::buffer(_readBuffer.header(), ReadBuffer::headerMaxLength), [this](const std::error_code ec, std::size_t bytes)
        {
            if (ec)
            {
                std::cerr << "Failed to read message! Shutting down socket" << std::endl;
                _socket.close();
                return;
            }

            doReadBody();
        });
    }

    void NetworkClient::doReadBody()
    {
        std::cout << "now reading body... bytes: " << _readBuffer.getTotalLength() << std::endl;
        _readBuffer.body().resize(_readBuffer.getTotalLength());
        asio::async_read(_socket, asio::buffer(_readBuffer.body(), _readBuffer.getTotalLength()), [this](const std::error_code ec, std::size_t bytes)
        {
            std::cout << "finished reading body" << std::endl;
            if (ec)
            {
                std::cerr << "Failed to read message! Shutting down socket" << std::endl;
                _socket.close();
                return;
            }

            const NetworkPacket packet = _readBuffer.collectPacket();
            std::cout << "got packet: " << packet.eventName << std::endl;

            doReadHeader();
            // TODO: do something with this packet
        });
    }

    void NetworkClient::_onWrite() // TODO: maybe call this doWrite also while ur at it also change _onConnect to a better name
    {
        const auto t = _writeBuffer.front();
        asio::async_write(_socket, asio::buffer(_writeBuffer.front().data(), _writeBuffer.front().size()), [this](std::error_code ec, std::size_t bytes)
        {
            if (ec)
            {
                std::cout << "Failed to write: " << ec.message() << std::endl;
                _socket.close();
                return;
            }
            _writeBuffer.pop_front();
            if (!_writeBuffer.empty()) _onWrite();
        });
    }


} // gp