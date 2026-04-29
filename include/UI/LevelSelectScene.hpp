//
// Created by guppy on 4/28/26.
//

#pragma once
#include "flow/Scene.hpp"
#include "SFML/Graphics/RenderWindow.hpp"


class LevelSelectScene : public flow::Scene
{
    sf::RenderWindow& _window;
    std::string _nextLevelPath;
public:
    explicit LevelSelectScene(sf::RenderWindow& window) : Scene("level-select"), _window(window)
    {
    }

    void initialize() override {}
    void update(float dt) override;
    void draw() override {}
    void onEnter() override {};
    void onExit() override {};
};