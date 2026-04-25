#include "flow/Scene.hpp"
namespace flow
{
	Scene::Scene(const std::string uuid) : _uuid(uuid) {}

	void Scene::onEnter() { return; }
	void Scene::onExit() { return; }

	std::string Scene::get_uuid() { return _uuid; }
}