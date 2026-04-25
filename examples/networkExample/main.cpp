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
        
        client.emit("exampleEvent", "test");
    });

    client.on<std::string>("exampleEvent", [](const std::string_view event)
    {
        std::cout << "got: " << event << std::endl;
    });
    
    std::cout << "Hello World!" << std::endl;

    while (true)
    {
        std::string msg;
        std::cin >> msg;

        client.emit("exampleEvent", msg);
    }
    
    NetworkManager::Stop();

    return 0;
}
