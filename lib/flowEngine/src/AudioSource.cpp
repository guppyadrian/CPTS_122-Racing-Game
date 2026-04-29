#include <iostream>

#include "flow/GameObject.hpp"
#include "flow/components/AudioSource.hpp"

namespace flow::audio
{
	AudioSource::AudioSource(std::string file) : mSound()
	{
		
		if (!mSound.openFromFile(file))
		{
			std::cerr << "Audio Souce failed to load audio!" << std::endl;
		}

		mSound.setDopplerFactor(10.f);
		mSound.setSpatializationEnabled(true);
		mSound.setMaxDistance(1000000.f);
		mSound.setMinDistance(20000.f);
		mSound.setAttenuation(0.f);
		mSound.setVolume(100.f);
		hasLoaded = true;
	}

	void AudioSource::init()
	{

	}

	void AudioSource::update(float dt)
	{
		sf::Vector2f pos = mGameObject->mTransform.getPosition();
		sf::Vector2f velocity = pos - lastPos;

		mSound.setPosition(sf::Vector3f(pos.x, pos.y, 0));
		mSound.setVelocity(sf::Vector3f(velocity.x, velocity.y, 0));

		//std::cout << "Audio Position: " << pos.x << ", " << pos.y << std::endl;

		lastPos = mGameObject->mTransform.getPosition();
	}
}