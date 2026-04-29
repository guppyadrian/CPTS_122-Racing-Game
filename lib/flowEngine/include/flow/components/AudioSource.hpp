#pragma once

#include <SFML/Audio.hpp>

#include "flow/Component.hpp"

namespace flow::audio
{
	class AudioSource : public Component
	{
		bool hasLoaded = false;

		sf::Vector2f lastPos;

		sf::SoundBuffer mBuffer;
		sf::Sound mSound;

	public:

		AudioSource(std::string file);

		void play() { if (hasLoaded) { mSound.play(); } }

		void pause() { mSound.pause(); }

		void stop() { mSound.stop(); }

		void loop(bool loop) { mSound.setLooping(loop); }

		void setMinDistance(float v) { mSound.setMinDistance(v); };
		void setMaxDistance(float v) { mSound.setMaxDistance(v); };
		void setAttenuation(float v) { mSound.setAttenuation(v); };
		void setPitch(float v) { mSound.setPitch(v); }

		void init() override;
		void update(float dt) override;
		void fixedUpdate() override {};
	};
}