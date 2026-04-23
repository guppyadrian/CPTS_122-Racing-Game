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
        std::vector<ServerConnection> _connections; // TODO: is this right??
        std::unordered_map<std::string, std::function<void(const std::vector<uint8_t>&)>> _listeners;
    public:
        explicit NetworkServer(uint16_t port);
        void listen();
        
        template<Serializable T, EventCallback<T> F>
        void on(const std::string& eventName, F callback);
        void on(const std::string& eventName, const std::function<void()>& callback);

    private:
        void doAccept();
        
    };

    template<Serializable T, EventCallback<T> F>
    void NetworkServer::on(const std::string &eventName, F callback)
    {
        _listeners[eventName] = [callback](const std::vector<uint8_t> &data)
        {
            callback(Deserialize<T>(data));
        };
    }
}
