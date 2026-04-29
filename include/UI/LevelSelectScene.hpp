//
// Created by guppy on 4/28/26.
//

#pragma once
#include "UIScene.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/RenderWindow.hpp"


class LevelSelectScene : public UIScene
{
    std::string _nextLevelPath;
    std::vector<std::string> _levels;
    int _levelSelected = 0;
    std::unique_ptr<sf::Font> _font;
public:
    explicit LevelSelectScene(sf::RenderWindow& window) : UIScene("level-select", window)
    {
    }

    void initialize() override {}
    void update(float dt) override;
    void draw() override;
    void onEnter() override;
    void onExit() override { _font.reset(); };
private:
    void handleInput(sf::Vector2f inputVector);
};