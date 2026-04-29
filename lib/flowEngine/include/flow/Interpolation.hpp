/**
 * @file Interpolation.hpp
 * @author Logan Rainchild
 * @brief Helper namespace for linear and rotational interpolation
 */

#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

namespace flow
{
	namespace interp
	{
		static sf::Color lerp(sf::Color a, sf::Color b, float t) {
			return sf::Color(
				static_cast<uint8_t>(a.r + (b.r - a.r) * t),
				static_cast<uint8_t>(a.g + (b.g - a.g) * t),
				static_cast<uint8_t>(a.b + (b.b - a.b) * t),
				static_cast<uint8_t>(a.a + (b.a - a.a) * t)
			);
		}

		static sf::Vector2f lerp(const sf::Vector2f& start, const sf::Vector2f& end, float t)
		{
			return start * (1.0f - t) + end * t;
		}

		static float lerp(float start, float end, float t)
		{
			return start * (1.0f - t) + end * t;
		}

		static float slerp(const float start, const float end, float t)
		{
			float d = end - start;
			const float PI = 3.14159265358979323846f;
			while (d > PI)  d -= 2.0f * PI;
			while (d < -PI) d += 2.0f * PI;
			return start + d * t;
		}
	}
}