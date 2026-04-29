#include <iostream>


#include "flow/components/AudioListener.hpp"
#include "flow/GameObject.hpp"


namespace flow::audio
{
	void AudioListener::init()
	{
		sf::Listener::setDirection({ 0.f, 0.f, -1.f }); // Facing "into" the screen
		sf::Listener::setUpVector({ 0.f, 1.f, 0.f });    // Head pointing "up" the Y axis
	}

	void AudioListener::update(float dt)
	{
		sf::Vector2f pos = mGameObject->mTransform.getPosition();
		sf::Vector2f velocity = pos - lastPos;

		sf::Listener::setPosition(sf::Vector3f(pos.x, pos.y, 0.01f));
		sf::Listener::setVelocity(sf::Vector3f(velocity.x, velocity.y, 0));

		//std::cout << "Listener Position: " << pos.x << ", " << pos.y << std::endl;

		lastPos = mGameObject->mTransform.getPosition();
	}
}