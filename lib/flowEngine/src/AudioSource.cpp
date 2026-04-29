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

		mSound.setDopplerFactor(1.f);
		mSound.setRelativeToListener(true);
		mSound.setSpatializationEnabled(true);
		mSound.setMaxDistance(10000.f);
		mSound.setMinDistance(1000.f);
		mSound.setAttenuation(1000.f);
		mSound.setVolume(50.f);
		hasLoaded = true;
	}

	void AudioSource::init()
	{

	}

	void AudioSource::update(float dt)
	{
		sf::Vector2f pos = mGameObject->mTransform.getPosition();
		sf::Vector2f velocity = pos - lastPos;

		mSound.setPosition(sf::Vector3f(pos.x, 0, pos.y));
		mSound.setVelocity(sf::Vector3f(velocity.x,0,velocity.y));

		lastPos = mGameObject->mTransform.getPosition();
	}
}