//
// Created by guppy on 4/29/26.
//

#pragma once
#include "ButtonArray.hpp"
#include "UIScene.hpp"
#include "SFML/Graphics/Font.hpp"


class LobbyScene : public UIScene
{
public:
    enum class State // state machine??? i lowkey don't know what a state machine is
    {
        Hosting,
        Hosted,
        Joining,
        Joined
    };
private:
    bool _hasInitialized = false;
    bool _connected{false}; // FIXME: does not update live rn... disconnects are very bad
    ButtonArray _buttons;
    State _state;
    int _idAccumulator = 0;
public:
    
    explicit LobbyScene(sf::RenderWindow& window, State state);
    
    ~LobbyScene() override;

    void initialize() override;

    void update(float dt) override;

    void draw() override;

    void onEnter() override;
    
    void onExit() override;
    
private:
    void handleInput(sf::Vector2f inputVector);
    
    
};
