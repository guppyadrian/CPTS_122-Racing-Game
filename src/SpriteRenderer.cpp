#include "SpriteRenderer.hpp"
#include "Renderer.hpp"

namespace flow
{
	SpriteRenderer::SpriteRenderer(std::string textureSrc): mTexture(), mSprite(mTexture)
	{
		if (!mTexture.loadFromFile(textureSrc)) {
			// Handle error (texture failed to load)
		}

		mSprite.setTexture(mTexture, true);
		//mSprite.setOrigin(mSprite.getTextureRect().size);

		Renderer::getGlobalRenderer().addSpriteRenderer(this);
	}

	SpriteRenderer::~SpriteRenderer()
	{
		Renderer::getGlobalRenderer().removeSpriteRenderer(this);
	}

	sf::Sprite& SpriteRenderer::getSprite()
	{
		return mSprite;
	}
}
