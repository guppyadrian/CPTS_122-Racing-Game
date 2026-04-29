/**
 * @file AudioSource.hpp
 * @author Logan Rainchild
 * @brief A Component that provides spatial output for gameObjects
 */


#pragma once

#include <SFML/Audio.hpp>

#include "flow/Component.hpp"

namespace flow::audio
{
	class AudioSource : public Component
	{
		bool hasLoaded = false;

		sf::Vector2f lastPos;

		sf::Music mSound;

	public:

		AudioSource(std::string file);

		void play() { if (hasLoaded) { mSound.play(); } }

		void pause() { mSound.pause(); }

		void stop() { mSound.stop(); }

		void loop(bool loop) { mSound.setLooping(loop); }
		void setLoopPoints(sf::Music::TimeSpan timespan) { mSound.setLoopPoints(timespan); }

		void setMinDistance(float v) { mSound.setMinDistance(v); };
		void setMaxDistance(float v) { mSound.setMaxDistance(v); };
		void setAttenuation(float v) { mSound.setAttenuation(v); };

		void setVolume(float v) { mSound.setVolume(v); }
		float getVolume() { return mSound.getVolume(); }

		void setPitch(float v) { mSound.setPitch(v); }

		sf::Music::Status getStatus()
		{
			return mSound.getStatus();
		}

		void init() override;
		void update(float dt) override;
		void fixedUpdate() override {};
	};
}