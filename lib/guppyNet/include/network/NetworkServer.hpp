//
// Created by guppy on 4/20/26.
//

#pragma once
#include "ServerConnection.hpp"
#include "asio/ip/tcp.hpp"
#include "network/Serialize.hpp"
#include "network/Shared.hpp"

using asio::ip::tcp;

namespace gp::network
{
    class NetworkServer
    {
    private:
        asio::io_context& _io;
        tcp::acceptor _acceptor;
        std::vector<std::shared_ptr<ServerConnection>> _connections; // TODO: is this right??
    public:
        explicit NetworkServer(uint16_t port);
        void listen();

        std::function<void(std::shared_ptr<ServerConnection>)> onConnection;

    private:
        void doAccept();
    };
}
