//
// Created by guppy on 4/28/26.
//

#include "UI/ButtonArray.hpp"

#include <iostream>

void ButtonArray::add(const std::string& texturePath, const sf::Vector2f pos, const sf::Vector2f scale)
{
    _buttons.push_back(std::make_unique<UIButton>(texturePath, pos, scale));
    if (_buttons.size() == 1) _buttons.back()->select();
}

void ButtonArray::next()
{
    if (_optionSelected < _buttons.size() - 1)
    {
        _buttons[_optionSelected++]->deselect();
        _buttons[_optionSelected]->select();
    }
}

void ButtonArray::prev()
{
    if (_optionSelected > 0)
    {
        _buttons[_optionSelected--]->deselect();
        _buttons[_optionSelected]->select(); // TODO: option to rollover!
    }
}

void ButtonArray::draw(sf::RenderWindow &window)
{
    int i = 0;
    for (const auto& button : _buttons)
    {
        window.draw(button->getSprite());
    }
}

void ButtonArray::update(const float dt)
{
    for (const auto& button : _buttons)
    {
        button->update(dt);
    }
}
