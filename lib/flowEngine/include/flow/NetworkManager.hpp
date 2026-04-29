//
// Created by guppy on 4/24/26.
//

#pragma once
#include <cstdint>
#include <string_view>

#include "network/NetworkClient.hpp"
#include "network/NetworkServer.hpp"

namespace flow
{
    using namespace gp;
    class NetworkManager
    {
    private:
        network::NetworkClient* _client{}; // got some weird issues so now this is purposely a "memory leak"
        network::NetworkServer* _server{};
        asio::io_context _io{};
    public:
        NetworkManager(const NetworkManager&) = delete;
        NetworkManager& operator=(const NetworkManager&) = delete;
        static NetworkManager& getGlobal() {
            static NetworkManager Instance;
            return Instance;
        }

        network::NetworkClient& getClient() { return *_client; }
        network::NetworkServer& getServer() { return *_server; }
        asio::io_context& io() { return _io; }

    private:
        NetworkManager()
        {
            _client = new network::NetworkClient();
            _server = new network::NetworkServer();
        }
    };
    
} // flow