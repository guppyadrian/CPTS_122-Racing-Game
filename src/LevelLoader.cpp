#include "levelLoader.hpp"
void LevelLoader::readFile(std::string fileUUID)
{
	std::ifstream file(fileUUID);
	if (!file.is_open())
	{
		std::cerr << "Failed to open file: " << fileUUID << std::endl;
		return;
	}

	_ss.str(""); //yes both "" and clear are needed; idk why
	_ss.clear();
	_ss << file.rdbuf();

	file.close();

	std::string line;

	std::string uuid;
	std::getline(_ss, uuid);

	int lvNum;
	std::getline(_ss, line); lvNum = std::stoi(line);

	sf::Color mainColor;
	int r, g, b, a;
	std::getline(_ss, line);
	sscanf(line.c_str(), "%d%d%d%d", &r, &g, &b, &a);
	mainColor = sf::Color(r, g, b, a);

	std::string bgFile;
	std::getline(_ss, bgFile);

	sf::Vector2f playerPos;
	float playerRot;
	std::getline(_ss, line);
	sscanf(line.c_str(), "%f,%f,%f", &playerPos.x, &playerPos.y, &playerRot);

	float gravity;
	std::getline(_ss, line); gravity = std::stof(line);

	std::getline(_ss, line); //blank line
	//rest should be handled in _init() for objects

	_init(gravity, uuid, bgFile, playerPos, playerRot, mainColor);
}