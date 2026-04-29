//
// Created by guppy on 4/28/26.
//

#pragma once
#include <vector>

#include "UIButton.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

// used for drawing multiple buttons in a menu and u wanna be able to select the buttons and stuff
class ButtonArray
{
    std::vector<std::unique_ptr<UIButton>> _buttons;
    int _optionSelected = 0;
    bool allowRollover = false; // TODO: implement
    
public:
    ButtonArray() = default;
    
    void add(const std::string& texturePath, sf::Vector2f pos, sf::Vector2f scale = {1.0f, 1.0f});
    void next();
    void prev();
    int selected() const { return _optionSelected; }
    
    void draw(sf::RenderWindow& window);
    void update(float dt);
};
