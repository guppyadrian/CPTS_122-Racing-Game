//
// Created by guppy on 4/27/26.
//

#include "UI/MenuScene.hpp"

#include "flow/SceneManager.hpp"
#include "UI/LevelSelectScene.hpp"
#include "UI/LobbyScene.hpp"

MenuScene::MenuScene(sf::RenderWindow &window)
    : UIScene("menu", window)
{
    const float y = _window.getSize().y / 2.0f;
    _mainMenuButtons.add("menu/playButton.png", {400, y}, {0.7f, 0.7f }); // play
    _mainMenuButtons.add("menu/lanButton.png", {1000, y}, { 0.7f, 0.7f }); // mutliplayer
    _mainMenuButtons.add("menu/quitButton.png", {1600, y}, { 0.7f, 0.7f }); // quit
    
    _lobbyButtons.add("menu/host.png", { 400, y}, {1.0f, 1.0f}); // host
    _lobbyButtons.add("menu/join.png",  { 1000, y}, {1.0f, 1.0f}); // join
    _lobbyButtons.add("menu/quitButton.png", {1600, y}, {0.5f, 0.5f}); // back
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
            if (_state == State::Menu)
                handleInputMain(getInputVector(keyPressed));
            else if (_state == State::Lobby)
                handleInputLobby(getInputVector(keyPressed));
        }
    }
    
    _mainMenuButtons.update(dt);
    _lobbyButtons.update(dt);
    if (!_queueNextScene.empty())
    {
        if (_queueNextScene == "level-select")
            flow::SceneManager::getGlobal().loadScene(std::make_unique<LevelSelectScene>(_window));
        else if (_queueNextScene == "multiplayer-host")
            flow::SceneManager::getGlobal().loadScene(std::make_unique<LobbyScene>(_window, LobbyScene::State::Hosting));
        else if (_queueNextScene == "multiplayer-join") // dude atp im just trying to get this done
            flow::SceneManager::getGlobal().loadScene(std::make_unique<LobbyScene>(_window, LobbyScene::State::Joining));
        else throw std::runtime_error("Main menu loading non existant scene ??? How is this possible!");
        flow::SceneManager::getGlobal().switchScene(_queueNextScene);
    }
}

void MenuScene::draw()
{
    _window.clear(sf::Color::Black);
    if (_state == State::Menu)
        _mainMenuButtons.draw(_window);
    else if (_state == State::Lobby)
        _lobbyButtons.draw(_window);
}

void MenuScene::handleInputMain(const sf::Vector2f inputVector) // taking in vector for possible controller support in the future
{
    if (inputVector.y > 0) // confirm button
    {
        switch (_mainMenuButtons.selected())
        {
            case 0: // play
                flow::SceneManager::getGlobal().loadScene(std::make_unique<LevelSelectScene>(_window));
                flow::SceneManager::getGlobal().switchScene("level-select");
                break;
            case 1: // multiplayer
                _state = State::Lobby;
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
        _mainMenuButtons.next();
    }
    else if (inputVector.x < 0)
    {
        _mainMenuButtons.prev();
    }
    
}

void MenuScene::handleInputLobby(const sf::Vector2f inputVector)
{
    if (inputVector.y > 0)
    {
        switch (_lobbyButtons.selected())
        {
            case 0: // host
                flow::SceneManager::getGlobal().loadScene(std::make_unique<LobbyScene>(_window, LobbyScene::State::Hosting));
                flow::SceneManager::getGlobal().switchScene("multiplayer-lobby");
                break;
            case 1: // join
                flow::SceneManager::getGlobal().loadScene(std::make_unique<LobbyScene>(_window, LobbyScene::State::Joining));
                flow::SceneManager::getGlobal().switchScene("multiplayer-lobby");
                break;
            case 2: // back
                _state = State::Menu;
                break;
            default: 
                break;
        }
    }
    if (inputVector.x > 0)
    {
        _lobbyButtons.next();
    } 
    else if (inputVector.x < 0)
    {
        _lobbyButtons.prev();
    }
}
