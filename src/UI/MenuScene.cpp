//
// Created by guppy on 4/27/26.
//

#include "UI/MenuScene.hpp"

#include "flow/SceneManager.hpp"
#include "UI/LevelSelectScene.hpp"

MenuScene::MenuScene(sf::RenderWindow &window)
    : UIScene("menu", window)
{
    const float y = static_cast<float>(_window.getSize().y) / 2 - 200;
    _buttons.add("menu/playButton.png", {100, y}, {0.3f, 0.3f}); // play
    _buttons.add("menu/playButton.png", {500, y}, {0.3f, 0.3f}); // mutliplayer
    _buttons.add("menu/quitButton.png", {900, y}, {0.3f, 0.3f}); // quit
}

void MenuScene::update(const float dt)
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
            handleInput(getInputVector(keyPressed));
        }
    }
    
    _buttons.update(dt);
    if (!_queueNextScene.empty())
    {
        if (_queueNextScene == "level-select")
            flow::SceneManager::getGlobal().loadScene(std::make_unique<LevelSelectScene>(_window));
        flow::SceneManager::getGlobal().switchScene(_queueNextScene);
    }
}

void MenuScene::draw()
{
    _window.clear(sf::Color::Black);
    
    _buttons.draw(_window);
}

void MenuScene::handleInput(const sf::Vector2f inputVector) // taking in vector for possible controller support in the future
{
    if (inputVector.y > 0) // confirm button
    {
        switch (_buttons.selected())
        {
            case 0: // play
                _queueNextScene = "level-select";
                break;
            case 1: // multiplayer
                _queueNextScene = "multiplayer-lobby";
                break;
            case 2: // quit
                _window.close(); // TODO: does this quit cleanly?
                break;
            default: 
                break;
        }
    }
    if (inputVector.x > 0) // TODO: deadzone?
    {
        _buttons.next();
    }
    else if (inputVector.x < 0)
    {
        _buttons.prev();
    }
    
}
