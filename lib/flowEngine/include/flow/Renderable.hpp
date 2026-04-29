/**
 * @file Renderable.hpp
 * @author Logan Rainchild
 * @brief A virtual for Components that can be rendered
 */

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