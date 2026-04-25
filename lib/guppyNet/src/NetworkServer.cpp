//
// Created by guppy on 4/20/26.
//

#include "network/NetworkServer.hpp"

#include <iostream>

#include "network/NetworkManager.hpp"

namespace gp::network
{
    NetworkServer::NetworkServer(const uint16_t port) : _io(NetworkManager::io()), _acceptor(_io, tcp::endpoint(tcp::v4(), port))
    {
    }

    void NetworkServer::listen()
    {
        // TODO???
        doAccept();
    }

    void NetworkServer::doAccept()
    {
        _acceptor.async_accept([this](const std::error_code ec, tcp::socket socket)
        {
            if (ec)
            {
                std::cerr << ec.value() << std::endl;
                socket.close(); // TODO: is this even necessary
                return;
            }

            std::cout << "new connection: " << socket.remote_endpoint() << std::endl;

            const auto conn = std::make_shared<ServerConnection>(std::move(socket));

            if (onConnection) onConnection(conn); // TODO: somewhere in the code there is a ByteBuffer vec{}, make is use ByteBuffer{} in one line

            _connections.push_back(conn);

            conn->start();
            
            doAccept();
        });
    }
} // gp


// on("connection", [](socket){});