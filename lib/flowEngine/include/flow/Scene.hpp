#pragma once
#include <vector>
#include <string>

namespace flow
{
	class Scene
	{
	private:
		std::string _uuid;
	public:
		Scene(const std::string &uuid);
		virtual ~Scene() = default;
		virtual void initialize() = 0;
		virtual void update(float dt) = 0;
		virtual void onEnter();
		virtual void onExit();

		std::string get_uuid();
	};
}