#pragma once

#include <SFML/Graphics.hpp>

#include "flow/Component.hpp"

namespace flow
{
	class Renderable : public Component
	{
	public:
		virtual const sf::Drawable& getDrawable() = 0;
	};
}