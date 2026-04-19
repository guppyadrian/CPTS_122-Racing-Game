#include "SceneManager.hpp"

SceneManager::SceneManager() : _curScene(nullptr), _sceneList() {}

SceneManager::~SceneManager()
{
	for (int i = 0; i < _sceneList.size(); i++)
	{
		this->removeScene(i);
	}
	_sceneList.shrink_to_fit();
}

void SceneManager::update()
{
	for (int i = 0; i < _sceneList.size(); i++)
	{
		_sceneList[i].update();
	}
}

//Return: true if success, false if failed
bool SceneManager::switchScene(const int index)
{
	if (index > _sceneList.size())
	{
		return false;
	}
	_curScene = &(_sceneList[index]);

	return true;
}

void SceneManager::addScene(Scene& newScene)
{
	_sceneList.push_back(newScene);
}

//Return: true if success, false if failed
bool SceneManager::removeScene(const int index)
{
	if (index > _sceneList.size())
	{
		return false;
	}
	_sceneList.erase(_sceneList.begin() + index);
	return true;
}