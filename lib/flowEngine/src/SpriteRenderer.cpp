#include "flow/components/SpriteRenderer.hpp"
#include "flow/Renderer.hpp"

namespace flow
{
	SpriteRenderer::SpriteRenderer(std::string textureSrc): mTexture(), mSprite(mTexture)
	{
		if (!mTexture.loadFromFile(textureSrc)) {
			// Handle error (texture failed to load)
		}

		mSprite.setTexture(mTexture, true);

		sf::FloatRect local = mSprite.getLocalBounds();
		mSprite.setOrigin(local.size * 0.5f);

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
