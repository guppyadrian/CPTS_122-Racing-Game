//
// Created by guppy on 4/20/26.
//

#pragma once
#include "ServerConnection.hpp"
#include "asio/ip/tcp.hpp"
#include "network/Serialize.hpp"
#include "network/NetworkManager.hpp" // pls dont delete! this is for forwarding manager

using asio::ip::tcp;

namespace gp::network
{
    // Shared pointer to a ServerConnection
    using Socket = std::shared_ptr<ServerConnection>;
    
    class NetworkServer
    {
    private:
        asio::io_context& _io;
        tcp::acceptor _acceptor;
        std::vector<Socket> _connections;
    public:
        explicit NetworkServer(uint16_t port);
        void listen();

        std::function<void(Socket)> onConnection;

    private:
        void doAccept();
    };
}
