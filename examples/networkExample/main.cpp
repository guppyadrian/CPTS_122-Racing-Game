//
// Created by guppy on 4/22/26.
//

#include <iostream>

#include <network/NetworkClient.hpp>
#include <network/NetworkManager.hpp>

using namespace gp::network;

int main()
{
    NetworkManager::Start();

    NetworkClient client;

    client.connect("localhost", 3000);

    client.on("connection", [&client]()
    {
        std::cout << "connected" << std::endl;
        client.emit("exampleEvent", std::string_view("blah"));
    });
    
    std::cout << "Hello World!" << std::endl;

    while (true)
    {
        
    }
    
    NetworkManager::Stop();

    return 0;
}
