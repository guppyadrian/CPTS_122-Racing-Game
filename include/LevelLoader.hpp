#pragma once //Include everything bc I said so
#include <iostream>
#include <memory>
#include <vector>
#include <utility>
#include <sstream>
#include <fstream>
#include <cstdio> //ITS BACK!

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Window.hpp>

#include <flow/Component.hpp>
#include <flow/GameObject.hpp>
#include <flow/components/SpriteRenderer.hpp>
#include <flow/Renderer.hpp>
#include <flow/components/Rigidbody.hpp>
#include <flow/PhysicsManager.hpp>
#include <flow/SceneManager.hpp>
#include <flow/LevelScene.hpp>
#include <flow/components/Camera.hpp>

#include "WallGenerator.hpp"
#include "Player.hpp"
#include "EndGoal.hpp"

class LevelLoader
{
private:
	std::stringstream _ss;
	void _init(const float& grav, const std::string& uuid, const std::string& bgFile,
		const sf::Vector2f& playerPos, const float& playerRot, const sf::Color& color);
public:
	LevelLoader() : _ss() {};
	~LevelLoader() = default;
	void readFile(std::string uuid);
	void onExit(); //ie. reset everything to default
};
