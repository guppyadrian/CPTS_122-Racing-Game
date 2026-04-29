//
// Created by guppy on 4/27/26.
//

#pragma once
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"

class UIButton
{
    sf::Texture _texture;
    sf::Sprite _sprite;
    bool _selected{false};
    sf::Vector2f _scale;
public:
    UIButton(const std::string& texturePath, sf::Vector2f pos, sf::Vector2f scale = {1, 1});
    
    void update(float dt);
    
    sf::Vector2f getPosition() const { return _sprite.getPosition(); }
    void setPosition(const sf::Vector2f pos) { _sprite.setPosition(pos); }
    
    sf::Sprite& getSprite() { return _sprite; }
    
    void select() { _selected = true; }
    void deselect() { _selected = false; }
};
