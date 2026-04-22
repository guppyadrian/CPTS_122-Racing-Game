#include "flow/LevelScene.hpp"

LevelScene::LevelScene() : _gameObjects() {}

void LevelScene::initialize()
{
	for (int i = 0; i < _gameObjects.size(); i++)
	{
		_gameObjects[i].init();
	}
}

void LevelScene::update(float dt)
{
	for (int i = 0; i < _gameObjects.size(); i++)
	{
		_gameObjects[i].update(dt);
	}
}