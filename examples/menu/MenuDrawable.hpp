#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>

class MenuDrawable : public sf::Drawable
{
public:
    void add(std::function<const sf::Drawable*()> getter);
    void clear();

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    std::vector<std::function<const sf::Drawable*()>> _drawables;
};