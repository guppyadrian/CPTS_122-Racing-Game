/**
 * @file LevelLoader.hpp
 * @author Johnathan Emanuelli
 * @brief Master loader, loads level files and sets up the entire level scene
 */

#pragma once //Include everything bc I said so
#include <sstream>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include "flow/LevelScene.hpp"

class LevelLoader
{
private:
	std::stringstream _ss;
	std::unique_ptr<flow::LevelScene> _init(const float& grav, const std::string& uuid, const std::string& bgFile,
		const sf::Vector2f& playerPos, const float& playerRot, const sf::Color& color, const int& lvNum, const std::string& audioFile, bool preview);
public:
	LevelLoader() = default;
	~LevelLoader() = default;
	std::unique_ptr<flow::LevelScene> readFile(const std::string& fileUUID, bool preview = false);
	void onExit(); //ie. reset everything to default
};
