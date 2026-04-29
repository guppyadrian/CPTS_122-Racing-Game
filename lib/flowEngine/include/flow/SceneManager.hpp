#pragma once
#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include <iostream>
#include "Scene.hpp"

namespace flow
{

	class SceneManager
	{
	private:
		Scene* _curScene;
		std::vector<std::unique_ptr<Scene>> _sceneList;

		SceneManager();
		~SceneManager();
	public:

		SceneManager(const SceneManager&) = delete;
		SceneManager& operator=(const SceneManager&) = delete;
		static SceneManager& getGlobal() {
			static SceneManager Instance;
			return Instance;
		}

		void update(float dt);
		void draw();
		bool switchScene(const std::string uuid);
		bool loadScene(std::unique_ptr<Scene> newScene);
		bool loadScene(std::string uuid);
		bool removeScene(const std::string uuid);
		Scene& getCurrentScene() { return *_curScene; }
	};
}