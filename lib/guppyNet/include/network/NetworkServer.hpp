//
// Created by guppy on 4/20/26.
//

#pragma once
#include "ServerConnection.hpp"
#include "asio/io_context.hpp"
#include "asio/ip/tcp.hpp"

using asio::ip::tcp;

namespace gp::network
{
    class NetworkServer
    {
    private:
        asio::io_context& _io;
        tcp::acceptor _acceptor;
        std::vector<ServerConnection> _connections; // TODO: is this right??
    public:
        explicit NetworkServer(uint16_t port);
        void listen();

    private:
        void doAccept();
    };
}
