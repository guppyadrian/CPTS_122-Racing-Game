#pragma once
#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include <iostream>
#include "Scene.hpp"


class SceneManager
{
private:
	Scene* _curScene;
	std::vector<std::unique_ptr<Scene>> _sceneList;

public:
	SceneManager();
	~SceneManager();
	void update();
	bool switchScene(const std::string uuid);
	bool loadScene(std::unique_ptr<Scene> newScene);
	bool loadScene(std::string uuid);
	bool removeScene(const std::string uuid);
};
	