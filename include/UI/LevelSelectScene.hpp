//
// Created by guppy on 4/28/26.
//

#pragma once
#include "UIScene.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include <SFML/Graphics/Sprite.hpp>


class LevelSelectScene : public UIScene
{
    bool _queueNextLevel{false};
    std::vector<std::string> _levels;
    std::vector<std::string> _levelPaths; // 4 vectors of stuff? no problem :)
    std::vector<std::unique_ptr<sf::Texture>> _thumbnailTextures;
    std::vector<sf::Sprite> _thumbnails;
    int _levelSelected = 0;
    bool _hasInitialized{false};
public:
    explicit LevelSelectScene(sf::RenderWindow& window) : UIScene("level-select", window)
    {
    }

    void initialize() override;
    void update(float dt) override;
    void draw() override;
    void loadingDraw() const;
    void onEnter() override;
    void onExit() override { _font.reset(); };
private:
    void handleInput(sf::Vector2f inputVector);
};