//
// Created by guppy on 4/27/26.
//

#pragma once

#include "ButtonArray.hpp"
#include "UIScene.hpp"
#include "flow/Scene.hpp"
#include "SFML/Graphics/RenderWindow.hpp"


class MenuScene : public UIScene
{
private:
    enum class State // state machine??? i lowkey don't know what a state machine is
    {
        Menu,
        Lobby
    };
    
    ButtonArray _mainMenuButtons;
    ButtonArray _lobbyButtons;
    std::string _queueNextScene;
    State _state{State::Menu};
public:
    explicit MenuScene(sf::RenderWindow& window);

    void initialize() override {};
    void update(float dt) override;
    void draw() override;
private:
    void handleInputMain(sf::Vector2f inputVector);
    void handleInputLobby(sf::Vector2f inputVector);
};
