#pragma once
#include <SFML/Audio.hpp>

namespace flow::audio
{
	class MusicManager
	{
	public:
		MusicManager(const MusicManager&) = delete;
		MusicManager& operator=(const MusicManager&) = delete;
		static MusicManager& getGlobal() {
			static MusicManager Instance;
			return Instance;
		}

		void play()
		{
			music.play();
		}
		void play(std::string str)
		{
			load(str);
			play();
			setLoop(true);
		}
		void load(std::string str)
		{
			music.openFromFile("assets/audio/" + str + ".ogg");
		}
		void pause() { music.pause(); }
		void stop() { music.stop(); }
		void setLoop(bool b) { music.setLooping(b); }

	private:
		sf::Music music;
		MusicManager():music() {}
	};
}