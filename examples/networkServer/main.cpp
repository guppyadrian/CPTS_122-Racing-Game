//
// Created by guppy on 4/22/26.
//

#include <network/NetworkManager.hpp>
#include <network/NetworkServer.hpp>

using namespace gp::network;

int main()
{
    NetworkManager::Start();

    NetworkServer server(3000);

    server.listen();

    while (true)
    {
        
    }
}
