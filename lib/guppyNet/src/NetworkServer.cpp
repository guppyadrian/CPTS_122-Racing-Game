//
// Created by guppy on 4/20/26.
//

#include "network/NetworkServer.hpp"

#include <iostream>

namespace gp::network
{
    NetworkServer::NetworkServer() : _io(NetworkManager::io())
    {
    }

    void NetworkServer::listen(const uint16_t port)
    {
        _acceptor.emplace(_io, tcp::endpoint(tcp::v4(), port));
        // TODO???
        doAccept();
    }

    void NetworkServer::stop()
    {
        _acceptor->close();
        _acceptor.reset();
        _connections.clear();
    }

    void NetworkServer::emit(const std::string &eventName, const ByteBuffer &data)
    {
        for (const auto& connection : _connections)
        {
            connection->emit(eventName, data);
        }
    }

    void NetworkServer::doAccept()
    {
        _acceptor.value().async_accept([this](const std::error_code ec, tcp::socket socket)
        {
            if (ec)
            {
                std::cerr << "Error accepting: " << ec.message() << std::endl;
                socket.close(); // TODO: is this even necessary
                return;
            }

            std::cout << "new connection: " << socket.remote_endpoint() << std::endl;

            const auto conn = std::make_shared<ServerConnection>(std::move(socket), []()
            {
                
            });

            if (onConnection) onConnection(conn);
            
            auto iter = _connections.insert(_connections.end(), conn);

            conn->_onClose = [iter, this](){ _connections.erase(iter); };
            
            conn->start();
            
            doAccept();
        });
    }
} // gp


// on("connection", [](socket){});