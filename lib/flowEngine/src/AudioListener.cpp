#include "flow/components/AudioListener.hpp"
#include "flow/GameObject.hpp"

namespace flow::audio
{
	void AudioListener::init()
	{

	}

	void AudioListener::update(float dt)
	{
		sf::Vector2f pos = mGameObject->mTransform.getPosition();
		sf::Vector2f velocity = pos - lastPos;

		sf::Listener::setPosition(sf::Vector3f(pos.x, 0, pos.y));
		sf::Listener::setVelocity(sf::Vector3f(velocity.x, 0, velocity.y));
		if (velocity.lengthSquared() > 0)
		{
			sf::Listener::setDirection(sf::Vector3f(velocity.x, 0, velocity.y).normalized());
		}

		lastPos = mGameObject->mTransform.getPosition();
	}
}