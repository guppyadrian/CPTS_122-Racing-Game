#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "components/SpriteRenderer.hpp"

namespace flow
{
	class Renderer
	{
		sf::RenderWindow* windowRef;
		std::vector<SpriteRenderer*> mActiveSprites;

		Renderer() {};


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
			windowRef = window;
		};
		void addSpriteRenderer(SpriteRenderer* sprite);
		void removeSpriteRenderer(SpriteRenderer* sprite);
		void drawAll();
		void setView(const sf::View& view);

	};
}