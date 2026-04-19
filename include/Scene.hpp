#pragma once
#include <vector>
#include "GameObject.hpp"

class Scene
{
public:
	Scene();
	~Scene() = default;
	virtual void initialize() = 0;
	virtual void update() = 0;
};

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