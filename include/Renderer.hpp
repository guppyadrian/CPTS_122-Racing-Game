#pragma once

#include <vector>
#include "SpriteRenderer.hpp"
#include <SFML/Graphics.hpp>

namespace flow
{
	class Renderer
	{
		//static Renderer Instance;

		sf::RenderWindow& windowRef;
		std::vector<SpriteRenderer*> mActiveSprites;

	public:

		Renderer(sf::RenderWindow& window) : windowRef(window) {};

		void addSprite(SpriteRenderer* sprite);
		void removeSprite(SpriteRenderer* sprite);
		void drawAll();

	};
}