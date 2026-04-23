//
// Created by guppy on 4/22/26.
//

#pragma once
#include "asio/ip/tcp.hpp"

using asio::ip::tcp;

namespace gp::network
{
    class ServerConnection
    {
    private:
        tcp::socket _socket;
    public:
        explicit ServerConnection(tcp::socket socket) : _socket(std::move(socket)) {}
    };
}
