#pragma once

#include "flow/Component.hpp"
#include "flow/GameObject.hpp"
#include <SFML/Graphics.hpp>

namespace flow
{
	class Camera : public Component
	{
		sf::View mView;

		Camera();
		Camera(const sf::View &view) : mView(view) {};

		void setViewSize(sf::Vector2f size)
		{
			mView.setSize(size);
		};

		sf::Vector2f getViewSize()
		{
			mView.getSize();
		}

		void update(float dt) override;
	};
}