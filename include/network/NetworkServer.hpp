//
// Created by guppy on 4/20/26.
//

#pragma once
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
        std::unordered_map<std::string, tcp::socket> _connections; // TODO: is this right??
    public:
        NetworkServer();

        void listen(uint16_t port);

    private:
        void doAccept();
    };
}
