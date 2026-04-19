#include "SpriteRenderer.hpp"

namespace flow
{
	SpriteRenderer::SpriteRenderer(std::string textureSrc): mTexture(), mSprite(mTexture)
	{
		if (!mTexture.loadFromFile(textureSrc)) {
			// Handle error (texture failed to load)
		}

		mSprite.setTexture(mTexture);
	}
	sf::Sprite& SpriteRenderer::getSprite()
	{
		return mSprite;
	}
}
