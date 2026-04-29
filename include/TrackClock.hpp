#pragma once
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include <iostream>

class TrackClock
{
public:
	TrackClock(sf::RenderWindow& win, sf::Font& f) : _window(win), _trackText(f), _finalText(f)
	{
		_trackText.setCharacterSize(60);
		_trackText.setFillColor(sf::Color::White);
		_trackText.setPosition({ win.getSize().x - 300.f, 0 });
		_times.reserve(10);
		_dt.reset();
		_finalClock.reset();
		_finalText.setCharacterSize(30);
		_finalText.setFillColor({255,255,0,0});
		_finalText.setPosition({ 0, win.getSize().y - 30.f });
		_finalText.setString("");
	}
	sf::Clock& get_clock()
	{
		return _trackClock;
	}

	void lapDone()
	{
		_times.push_back(_lapClock.getElapsedTime().asMilliseconds());
		_lastLap = _lapClock.getElapsedTime().asMilliseconds();
		_dt.restart();
		_lapClock.restart();
		_lapsLeft--;

		_finalText.setPosition({ 0, _window.getSize().y - 50.f  - 30.f * _times.size()});
		
		std::string s;
		for (int i = 0; i < _times.size(); i++)
		{
			s.append("Lap " + std::to_string(i + 1) + ":  " +
				std::to_string(_times[i] /1000) + ":" + 
				std::to_string(_times[i] % 1000) + '\n');
		}
		s.append("Total: " + std::to_string(_trackClock.getElapsedTime().asMilliseconds() / 1000) + ":" + std::to_string(_trackClock.getElapsedTime().asMilliseconds() % 1000));
		_finalText.setString(s);
		_finalClock.start();
		_finalText.setFillColor({ 255,255,0,255 });
	}
	void reset()
	{
		_dt.reset();
		_lapClock.restart();
		_trackClock.restart();
		_times.clear();
		_lastLap = 0;
	}
	void update()
	{
		int t;
		if (_lapsLeft == _lapsOG )
		{
			t = 15000;
		}
		else
		{
			t = 5000;
		}
		if (_finalClock.isRunning() && _finalClock.getElapsedTime().asMilliseconds() > t)
		{
			_finalClock.reset();
			_finalText.setFillColor({ 255,255,0,0 });
		}
		//set color
		if ((_dt.getElapsedTime().asMilliseconds() % 1000 < 500) && _dt.isRunning())
		{
			_trackText.setFillColor({ 255,255,0,255 });
		}
		else
		{
			_trackText.setFillColor({ 255,255,255,255 });
		}
		if (_dt.isRunning())
		{
			int tSec = _lastLap / 1000;
			int tMs = _lastLap % 1000;

			_trackText.setString(std::to_string(tSec) + ":" + std::to_string(tMs) + "\nLap" + std::to_string(_lapsOG - _lapsLeft + 1) + ":" + std::to_string(_lapsOG));
		}
		else
		{
			int tSec = _trackClock.getElapsedTime().asMilliseconds() / 1000;
			int tMs = _trackClock.getElapsedTime().asMilliseconds() % 1000;
			_trackText.setString(std::to_string(tSec) + ":" + std::to_string(tMs) + "\nLap" + std::to_string(_lapsOG - _lapsLeft + 1) + ":" + std::to_string(_lapsOG));
		}
		if (_dt.getElapsedTime().asMilliseconds() > 2000)
		{
			_dt.reset();
		}

	}

	sf::Text& getText()
	{
		return _trackText;
	}

	sf::Text& getFinalText()
	{
		return _finalText;
	}

	int _lapsLeft = 1;
	int _lapsOG = 1;
private:
	sf::Clock _trackClock;
	std::vector<int> _times;
	sf::Clock _lapClock;
	int _lastLap = 0;
	sf::Clock _dt;
	sf::RenderWindow& _window;
	sf::Text _trackText;
	sf::Text _finalText;
	sf::Clock _finalClock;
};