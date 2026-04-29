/**
 * @file AudioListener.hpp
 * @author Logan Rainchild
 * @brief A Component that provides spatial listening for gameObjects
 */

#pragma once

#include <SFML/Audio.hpp>

#include "flow/Component.hpp"

namespace flow::audio
{
	class AudioListener : public Component
	{
		sf::Vector2f lastPos;

	public:

		AudioListener() {};

		void init() override;
		void update(float dt) override;
		void fixedUpdate() override {};
	};
}