#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "components/SpriteRenderer.hpp"
#include "Renderable.hpp"

namespace flow
{
	class Renderer
	{
		sf::RenderWindow* mWindowRef;
		std::vector<Renderable*> mActiveSprites;

		sf::RenderTexture mainScene;
		sf::RenderTexture brightPass;
		sf::RenderTexture ping;
		sf::RenderTexture pong;

		sf::Shader brightShader;
		sf::Shader blurShader;
		sf::Shader cromeAbShader;
		sf::Shader scanLines;
		sf::Shader crtDistortion;

		sf::Vector2f mCromeAbOffset = {0.01f, 0.001f};

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
			ping = sf::RenderTexture(mWindowRef->getSize());
			pong = sf::RenderTexture(mWindowRef->getSize());
		};

		void addRenderable(Renderable* renderable);
		void removeRenderable(Renderable* renderable);
		void drawAll();
		void setView(const sf::View& view);
		void setChromaticAberration(sf::Vector2f offset)
		{
			mCromeAbOffset = offset;
		}

	};
}