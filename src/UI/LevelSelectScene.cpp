//
// Created by guppy on 4/28/26.
//

#include "UI/LevelSelectScene.hpp"

#include "LevelLoader.hpp"

void LevelSelectScene::update(float dt)
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
            const auto keyPressed = event->getIf<sf::Event::KeyPressed>()->code;
            sf::Vector2f inputVector; // TODO: controller support
            if (keyPressed == sf::Keyboard::Key::D || keyPressed == sf::Keyboard::Key::Right) inputVector.x += 1;
            if (keyPressed == sf::Keyboard::Key::A || keyPressed == sf::Keyboard::Key::Left) inputVector.x -= 1;
            if (keyPressed == sf::Keyboard::Key::Space) inputVector.y += 1; // TODO: move to a getInputVector() function
            
            if (inputVector.y > 0)
            {
                _nextLevelPath = "Test";
            }
        }
    }
    
    if (!_nextLevelPath.empty())
        LevelLoader().readFile(_nextLevelPath);
}
