//
// Created by guppy on 4/27/26.
//

#pragma once
#include <memory>

#include "SFML/Graphics/Shape.hpp"

class UIElement
{
private:
    std::unique_ptr<sf::Shape> shape;
public:
    explicit UIElement(std::unique_ptr<sf::Shape>&& shape) : shape(std::move(shape)) {}
};
