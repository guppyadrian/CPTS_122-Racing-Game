// PA9.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

#include "GameObject.hpp"
#include "SpriteRenderer.hpp"
#include "Renderer.hpp"

int main()
{

    sf::RenderWindow window(sf::VideoMode({600, 400}), "Game");

    flow::Renderer fRenderer(window);

    std::vector<std::unique_ptr<flow::GameObject>> gameobjects; // replace with scene management

    for (int i = 0; i < 20; i++)
    {
        gameobjects.push_back(std::make_unique<flow::GameObject>()); // make a new GameObject
        gameobjects.back()->mTransform.setPosition(sf::Vector2f(i * 10,i * 10));
        gameobjects.back()->mTransform.setRotationDeg(i * 18);
        gameobjects.back()->mTransform.setScale(sf::Vector2f(1.f, 1.f));
        auto srComponent = std::make_unique<flow::SpriteRenderer>(std::string("assets/jonah.png")); // create a sprite renderer component
        flow::SpriteRenderer* srPtr = srComponent.get(); // keep a raw pointer for registration with the renderer
        gameobjects.back()->addComponent(std::move(srComponent)); // move the component into the object
        fRenderer.addSprite(srPtr); // register the sprite with the renderer so it will be drawn
    }

    // move to scene manager
    for (int i = 0; i < 20; i++)
    {
        gameobjects[i]->init();
    }

    while (window.isOpen())
    {
        // SFML in this workspace uses an optional-style pollEvent that returns
        // std::optional<sf::Event>. Use that form to handle events.
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // move to scene manager
        for (int i = 0; i < 20; i++)
        {
            gameobjects[i]->update();
        }

        window.clear();
        fRenderer.drawAll();
        window.display();
    }


}
