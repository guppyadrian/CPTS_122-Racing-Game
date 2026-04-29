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
			if (!_isOpen) return;
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
			music = sf::Music();
			_isOpen = music.openFromFile("assets/audio/" + str);
		}
		void pause() { if (!_isOpen) return; music.pause(); }
		void stop() { if (!_isOpen) return; music.stop(); }
		void setLoop(bool b) { if (!_isOpen) return; music.setLooping(b); }

	private:
		sf::Music music;
		bool _isOpen;
		MusicManager():music() {}
	};
}