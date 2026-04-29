/**
 * @file Camera.hpp
 * @author Logan Rainchild
 * @brief A Component for gameObjects that lets you manipulate the camera ouput rect scale and position
 */


#pragma once

#include "flow/Component.hpp"
#include "flow/GameObject.hpp"
#include <SFML/Graphics.hpp>

namespace flow
{
	class Camera : public Component
	{
	protected:
		sf::View mView;
	public:
		//Camera();
		explicit Camera(const sf::View &view) : mView(view) {};

		void setViewSize(const sf::Vector2f size)
		{
			mView.setSize(size);
		};

		sf::Vector2f getViewSize() const
		{
			return mView.getSize();
		}

		void init() override {};
		void update(float dt) override;
		void fixedUpdate() override {};
	};
}