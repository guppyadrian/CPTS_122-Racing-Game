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
    ButtonArray _buttons;
    std::string _queueNextScene;
public:
    explicit MenuScene(sf::RenderWindow& window);

    void initialize() override {};
    void update(float dt) override;
    void draw() override;
private:
    void handleInput(sf::Vector2f inputVector);
};
