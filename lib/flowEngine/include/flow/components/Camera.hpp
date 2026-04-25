#pragma once

#include "flow/Component.hpp"
#include "flow/GameObject.hpp"
#include <SFML/Graphics.hpp>

namespace flow
{
	class Camera : public Component
	{
		sf::View mView;
	public:
		//Camera();
		Camera(const sf::View &view) : mView(view) {};

		void setViewSize(sf::Vector2f size)
		{
			mView.setSize(size);
		};

		sf::Vector2f getViewSize()
		{
			return mView.getSize();
		}

		void init() override {};
		void update(float dt) override;
		void fixedUpdate() override {};
	};
}