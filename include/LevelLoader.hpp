#pragma once //Include everything bc I said so
#include <sstream>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>

class LevelLoader
{
private:
	std::stringstream _ss;
	void _init(const float& grav, const std::string& uuid, const std::string& bgFile,
		const sf::Vector2f& playerPos, const float& playerRot, const sf::Color& color, const int& lvNum);
public:
	LevelLoader() : _ss() {};
	~LevelLoader() = default;
	void readFile(std::string uuid);
	void onExit(); //ie. reset everything to default
};
