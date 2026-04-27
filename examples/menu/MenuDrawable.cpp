#include "MenuDrawable.hpp"

void MenuDrawable::add(std::function<const sf::Drawable*()> getter)
{
    _drawables.push_back(getter);
}

void MenuDrawable::clear()
{
    _drawables.clear();
}

void MenuDrawable::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (const auto& getter : _drawables)
    {
        const sf::Drawable* d = getter();
        if (d) target.draw(*d, states);
    }
}