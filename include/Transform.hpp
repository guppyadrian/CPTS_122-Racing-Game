#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/Transformable.hpp>

namespace flow
{
	class Transform : public sf::Transformable
	{
	public:

		void setRotationRad(float radians);
		void setRotationDeg(float degrees);
		float getRotationRad();
		float getRotationDeg();
	};
}