#pragma once

#include "flow/Component.hpp"
#include "flow/GameObject.hpp"
#include "flow/Renderable.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <string>

namespace flow
{
	class SpriteRenderer : public Renderable
	{
		sf::Texture mTexture;
		sf::Sprite mSprite;
	public:
		SpriteRenderer(std::string textureSrc);
		SpriteRenderer(const sf::Texture& texture);
		~SpriteRenderer();

		sf::Sprite& getSprite();

		const sf::Drawable& getDrawable() override;

		// implement Component interface
		void init() override {};
		void update(float dt) override {};
		void fixedUpdate() override {};
	};
}