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

	void SceneManager::draw()
	{
		_curScene->draw();
		
		// draw happens last, so safe to change scene atp
		if (_queueNextScene >= 0) _switchScene();
	}

	//Return: true if success, false if failed
	bool SceneManager::switchScene(const std::string& uuid, const bool deleteOld)
	{
		std::cout << "switchScene(): " << uuid << std::endl;
		_deleteOld = deleteOld;
		for (int i = 0; i < _sceneList.size(); i++)
		{
			if (_sceneList[i]->get_uuid() == uuid)
			{
				_queueNextScene = i;
				if (_curScene == nullptr)
				{
					_switchScene();
				}
				return true;
			}
		}
		return false;
	}

	void SceneManager::_switchScene()
	{
		const auto nextScene = _sceneList[_queueNextScene].get();
		if (_curScene && _deleteOld)
		{
			_curScene->onExit();
			removeScene(_curScene->get_uuid());
		}
		_curScene = nextScene;
		_curScene->onEnter();
		_queueNextScene = -1;
	}

	bool SceneManager::loadScene(std::unique_ptr<Scene> newScene)
	{
		for (const auto& scene : _sceneList)
		{
			if (scene->get_uuid() == newScene->get_uuid()) return false;
		}
		_sceneList.push_back(std::move(newScene));
		return true;
	}

	//Used for 
	bool SceneManager::loadScene(const std::string& uuid)
	{
		std::ifstream inFile;
		inFile.open(uuid);
		if (!inFile.is_open()) return false;
		//#TODO: Load file stuff here

		return true;
	}

	//Return: true if success, false if failed
	bool SceneManager::removeScene(const std::string& uuid)
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