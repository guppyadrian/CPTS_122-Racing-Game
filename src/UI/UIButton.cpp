//
// Created by guppy on 4/27/26.
//

#include "UI/UIButton.hpp"

UIButton::UIButton(const std::string& texturePath, const sf::Vector2f pos, const sf::Vector2f scale)
    : _texture("assets/" + texturePath), _sprite(_texture), _scale(scale)
{
    _sprite.setPosition(pos);
    _sprite.setScale(scale);
    _sprite.setOrigin(_sprite.getLocalBounds().getCenter());
}

void UIButton::update(const float)
{
    const sf::Vector2f targetScale = _scale * (_selected ? 1.3f : 1.0f);
    
    if (_sprite.getScale() != targetScale)
    {
        _sprite.setScale(targetScale);
        _sprite.setOrigin(_sprite.getLocalBounds().getCenter());
    }
}
