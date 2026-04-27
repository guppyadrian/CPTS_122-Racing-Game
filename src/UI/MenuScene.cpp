//
// Created by guppy on 4/27/26.
//

#include "UI/MenuScene.hpp"

#include <iostream>

void MenuScene::update(float dt)
{
    while (const std::optional event = _window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            _window.close();
            break;
        }
        if (event->is<sf::Event::KeyPressed>())
        {
            const auto keyPressed = event->getIf<sf::Event::KeyPressed>();
            std::cout << "key pressed " << std::endl;
            
        }
    }
}

void MenuScene::draw() const
{
    _window.clear(sf::Color::Black);
    
    _window.draw(_startButton);
    _window.draw(_quitButton);
    _window.draw(_multiplayerButton);
    
    _window.display();
}
