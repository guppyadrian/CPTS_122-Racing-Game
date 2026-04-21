//
// Created by guppy on 4/20/26.
//

#include "network/NetworkServer.hpp"

#include <iostream>

#include "network/NetworkManager.hpp"

namespace gp::network
{
    NetworkServer::NetworkServer() : _io(NetworkManager::io()), _acceptor(_io)
    {
    }

    void NetworkServer::listen(uint16_t port)
    {
        
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

            doAccept();
        });
    }
} // gp


// on("connection", [](socket){});