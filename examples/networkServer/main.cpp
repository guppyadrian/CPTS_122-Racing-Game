//
// Created by guppy on 4/22/26.
//

#include <iostream>
#include <network/NetworkManager.hpp>
#include <network/NetworkServer.hpp>

using namespace gp::network;

int main()
{
    NetworkManager::Start();

    NetworkServer server(3000);

    server.on("connection", []()
    {
        std::cout << "hi" << std::endl;
    });

    server.listen();

    while (true)
    {
        
    }
}
