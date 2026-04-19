#pragma once
#include <vector>
#include <string>

class Scene
{
private:
	std::string _uuid;
public:
	Scene();
	Scene(const std::string uuid);
	virtual ~Scene() = default;
	virtual void initialize() = 0;
	virtual void update() = 0;
	virtual void onEnter();
	virtual void onExit();
	
	inline std::string get_uuid();
};