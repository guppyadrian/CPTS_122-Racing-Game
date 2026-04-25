#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "components/SpriteRenderer.hpp"

namespace flow
{
	class Renderer
	{
		sf::RenderWindow* mWindowRef;
		std::vector<SpriteRenderer*> mActiveSprites;

		sf::RenderTexture mainScene;
		sf::RenderTexture brightPass;
		sf::RenderTexture blurPing;
		sf::RenderTexture blurPong;
		sf::RenderTexture chromaticAberration;

		sf::Shader brightShader;
		sf::Shader blurShader;
		sf::Shader cromeAbShader;
		sf::Shader scanLines;
		sf::Shader crtDistortion;

		sf::Vector2f mCromeAbOffset;

		Renderer();

	public:

		// Singleton Pattern
		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		static Renderer& getGlobalRenderer() {
			static Renderer Instance;
			return Instance;
		}

	
		void attachWindow(sf::RenderWindow* window)
		{
			mWindowRef = window;
			mainScene = sf::RenderTexture(mWindowRef->getSize());
			brightPass = sf::RenderTexture(mWindowRef->getSize());
			blurPing = sf::RenderTexture(mWindowRef->getSize());
			blurPong = sf::RenderTexture(mWindowRef->getSize());
			chromaticAberration = sf::RenderTexture(mWindowRef->getSize());
		};

		void addSpriteRenderer(SpriteRenderer* sprite);
		void removeSpriteRenderer(SpriteRenderer* sprite);
		void drawAll();
		void setView(const sf::View& view);
		void setChromaticAberration(sf::Vector2f offset)
		{
			mCromeAbOffset = offset;
		}

	};
}