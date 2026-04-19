#pragma once
#include <vector>
#include <string>
#include "GameObject.hpp"

class Scene
{
public:
	Scene();
	virtual ~Scene() = default;
	virtual void initialize() = 0;
	virtual void update() = 0;
	virtual void onEnter();
	virtual void onExit();
	
	std::string uuid;
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