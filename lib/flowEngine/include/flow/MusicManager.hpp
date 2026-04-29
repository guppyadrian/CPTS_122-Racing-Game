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

		void play(std::string str)
		{
			
			if (music.openFromFile("assets/audio/" + str + ".ogg"))
			{
				music.play();
			}
		}
		void pause() { music.pause(); }
		void stop() { music.stop(); }
		void setLoop(bool b) { music.setLooping(b); }

	private:
		sf::Music music;
		MusicManager():music() {}
	};
}