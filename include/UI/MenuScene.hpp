//
// Created by guppy on 4/27/26.
//

#pragma once
#include <memory>

#include "flow/Scene.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"


class MenuScene : public flow::Scene
{
private:
    sf::RenderWindow& _window;
    sf::RectangleShape _startButton;
    sf::RectangleShape _quitButton;
    sf::RectangleShape _multiplayerButton;
public:
    explicit MenuScene(sf::RenderWindow& window) : Scene("menu"), _window(window)
    {
    }

    void initialize() override {};
    void update(float dt) override;
    void onEnter() override {}
    void onExit() override {}

private:
    void draw() const;
};
