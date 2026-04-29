//
// Created by guppy on 4/20/26.
//

#pragma once
#include <list>

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
        std::optional<tcp::acceptor> _acceptor;
        std::list<Socket> _connections;
    public:
        NetworkServer();
        void listen(uint16_t port);
        
        void stop();

        std::function<void(Socket)> onConnection;
        template <Serializable T>
        void emit(const std::string& eventName, const T& data);
        void emit(const std::string& eventName, const ByteBuffer& data);

    private:
        void doAccept();
        void onClose();
    };

    template<Serializable T>
    void NetworkServer::emit(const std::string &eventName, const T &data)
    {
        for (auto &connection : _connections)
        {
            connection->emit(eventName, data);
        }
    }
}
