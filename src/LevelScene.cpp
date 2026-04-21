#include "LevelScene.hpp"

LevelScene::LevelScene() : _gameObjects() {}

void LevelScene::initialize()
{
	for (int i = 0; i < _gameObjects.size(); i++)
	{
		_gameObjects[i].initialize(); //TODO
	}
}

void LevelScene::update()
{
	for (int i = 0; i < _gameObjects.size(); i++)
	{
		_gameObjects[i].update(); //TODO
	}
}