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

		Renderer::getGlobalRenderer().addRenderable(this);
	}

	SpriteRenderer::SpriteRenderer(const sf::Texture& texture) : mTexture(texture), mSprite(mTexture)
	{
		mSprite.setTexture(mTexture, true);

		sf::FloatRect local = mSprite.getLocalBounds();
		mSprite.setOrigin(local.size * 0.5f);

		Renderer::getGlobalRenderer().addRenderable(this);
	}

	SpriteRenderer::SpriteRenderer(const sf::Texture& texture, sf::IntRect rect) : mTexture(texture), mSprite(mTexture)
	{
		mTexture.setRepeated(true);
		mSprite.setTexture(mTexture, true);

		mSprite.setTextureRect(rect);

		sf::FloatRect local = mSprite.getLocalBounds();
		mSprite.setOrigin(local.size * 0.5f);

		Renderer::getGlobalRenderer().addRenderable(this);
	}

	SpriteRenderer::~SpriteRenderer()
	{
		Renderer::getGlobalRenderer().removeRenderable(this);
	}

	sf::Sprite& SpriteRenderer::getSprite()
	{
		return mSprite;
	}

	const sf::Drawable& SpriteRenderer::getDrawable()
	{
		return mSprite;
	}
}
