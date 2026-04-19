#include "SceneManager.hpp"

SceneManager::SceneManager() : _curScene(nullptr), _sceneList() {}

SceneManager::~SceneManager()
{
	_sceneList.clear();
	_sceneList.shrink_to_fit();
}

void SceneManager::update()
{
	_curScene->update();
}

//Return: true if success, false if failed
bool SceneManager::switchScene(const std::string uuid)
{
	for (int i = 0; i < _sceneList.size(); i++)
	{
		if (_sceneList[i]->uuid == uuid)
		{
			if(_curScene) _curScene->onExit();
			_curScene = _sceneList[i].get();
			_curScene->onEnter();
			return true;
		}
	}
	return false;
}

void SceneManager::addScene(std::unique_ptr<Scene> newScene)
{
	_sceneList.push_back(newScene);
}

//Return: true if success, false if failed
bool SceneManager::removeScene(const std::string uuid)
{
	for (int i = 0; i < _sceneList.size(); i++)
	{
		if (_sceneList[i]->uuid == uuid)
		{
			_sceneList.erase(_sceneList.begin() + i);
			return true;
		}
	}
	return false;
}