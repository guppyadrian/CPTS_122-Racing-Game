#include "flow/SceneManager.hpp"
#include <utility>

namespace flow
{

	SceneManager::SceneManager() : _curScene(nullptr), _sceneList() {}

	SceneManager::~SceneManager()
	{
		_sceneList.clear();
		_sceneList.shrink_to_fit();
	}

	void SceneManager::update(float dt)
	{
		_curScene->update(dt);
	}

	//Return: true if success, false if failed
	bool SceneManager::switchScene(const std::string uuid)
	{
		for (int i = 0; i < _sceneList.size(); i++)
		{
			if (_sceneList[i]->get_uuid() == uuid)
			{
				if (_curScene) _curScene->onExit();
				_curScene = _sceneList[i].get();
				_curScene->onEnter();
				return true;
			}
		}
		return false;
	}

	bool SceneManager::loadScene(std::unique_ptr<Scene> newScene)
	{
		_sceneList.push_back(std::move(newScene));
		return true;
	}

	//Used for 
	bool SceneManager::loadScene(std::string uuid)
	{
		std::ifstream inFile;
		inFile.open(uuid);
		if (!inFile.is_open()) return false;
		//#TODO: Load file stuff here

		return true;
	}

	//Return: true if success, false if failed
	bool SceneManager::removeScene(const std::string uuid)
	{
		for (int i = 0; i < _sceneList.size(); i++)
		{
			if (_sceneList[i]->get_uuid() == uuid)
			{
				_sceneList.erase(_sceneList.begin() + i);
				return true;
			}
		}
		return false;
	}
}