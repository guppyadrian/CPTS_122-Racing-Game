#pragma once

#include "flow/Component.hpp"
#include "flow/GameObject.hpp"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <string>

namespace flow
{
	class SpriteRenderer : public Component
	{
		sf::Texture mTexture;
		sf::Sprite mSprite;
	public:
		SpriteRenderer(std::string textureSrc);
		~SpriteRenderer();

		sf::Sprite& getSprite();

		// implement Component interface
		void init() override {};
		void update(float dt) override {};
		void fixedUpdate() override {};
	};
}