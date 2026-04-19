// PA9.cpp : Defines the entry point for the application.
//

#include <iostream>

#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Window.hpp"
#include "network/NetworkClient.hpp"
#include "network/NetworkManager.hpp"

using namespace gp::network;

int main()
{
    NetworkManager::Start();
    NetworkClient client;

    client.connect("localhost", 3000);

    sf::RenderWindow window(sf::VideoMode({600, 400}), "Game");

    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    bool sent = false;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();

        if (client.connected() && !sent)
        {
            client.emit("hello", std::string_view("world..."));
            sent = true;
        }
    }

    NetworkManager::Stop();
}
