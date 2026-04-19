//
// Created by guppy on 4/18/2026.
//

#include <iostream>

#include "asio/connect.hpp"
#include "asio/read.hpp"

#include "network/NetworkClient.hpp"

#include "asio/write.hpp"

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

    void NetworkClient::send(std::string_view eventName, std::vector<uint8_t> data)
    {
        // packet format: [uint32_t bytesSize][uint16_t eventNameSize][char* eventName][uint8_t bytes]
        std::vector<uint8_t> packet;
        packet.reserve(data.size() + sizeof(uint32_t) + sizeof(uint16_t) + eventName.size()); // should be what I need to reserve right?

        // append sizes
        AppendAsBytes(packet, static_cast<uint32_t>(data.size()));
        AppendAsBytes(packet, static_cast<uint16_t>(eventName.size())); // TODO: checking to make sure this doesn't overflow

        packet.insert(packet.end(), eventName.begin(), eventName.end());
        packet.insert(packet.end(), data.begin(), data.end());


        asio::post(_io, [this, packet]()
        {
            const bool writeInProgress = !_writeBuffer.empty();
            _writeBuffer.push_back(packet);
            if (!writeInProgress) _onWrite();
        });
    }

    void NetworkClient::_onConnect()
    {
        _socket.async_read_some(asio::buffer(_readBuffer),
            [this](const std::error_code ec, const std::size_t bytes)
            {
                if (ec)
                {
                    std::cout << "Failed to read: " << ec.message() << std::endl;
                    return;
                }

                const std::string msg(_readBuffer.data(), bytes);
                std::cout << "Received: " << msg << std::endl;

                _onConnect();
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