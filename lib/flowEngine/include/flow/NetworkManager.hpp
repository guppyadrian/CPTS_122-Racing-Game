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
        network::NetworkClient _client{};
        network::NetworkServer _server{};
    public:
        NetworkManager(const NetworkManager&) = delete;
        NetworkManager& operator=(const NetworkManager&) = delete;
        static NetworkManager& getGlobal() {
            static NetworkManager Instance;
            return Instance;
        }

        network::NetworkClient& getClient() { return _client; }
        network::NetworkServer& getServer() { return _server; }

    private:
        NetworkManager() = default;
    };
    
} // flow