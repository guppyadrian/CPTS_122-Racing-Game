#pragma once
#include "Scene.hpp"
#include "GameObject.hpp"


class LevelScene : public Scene
{
private:
	std::vector<GameObject> _gameObjects;
public:
	LevelScene();
	~LevelScene() = default;
	void initialize();
	void update();
};