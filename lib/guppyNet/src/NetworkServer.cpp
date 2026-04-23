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

    void NetworkServer::on(const std::string &eventName, const std::function<void()> &callback)
    { // TODO: error checking... also in the other on() overload
        _listeners[eventName] = [callback](const std::vector<uint8_t>&) { callback(); };
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

            ServerConnection conn(std::move(socket)); // TODO: need std::move?

            constexpr std::vector<uint8_t> nothing; // TODO: how can I not do this
            if (_listeners.contains("connection")) _listeners["connection"](nothing);

            _connections.push_back(std::move(conn));
            
            doAccept();
        });
    }
} // gp


// on("connection", [](socket){});