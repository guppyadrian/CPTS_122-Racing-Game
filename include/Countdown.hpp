#pragma once

#include <chrono>

#include <flow/Component.hpp>
#include <flow/GameObject.hpp>

#include "TrackClock.hpp"
#include "Player.hpp"


class Countdown : public flow::Component
{
	long long mStartTimestamp;

public:
	Countdown(long long startTimestamp) : mStartTimestamp(startTimestamp) {};

	void init() override {};
	void update(float dt) override
	{
		auto time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		if (mStartTimestamp <= time)
		{
			mGameObject->getComponent<PlayerController>()->reset();
			if (TrackClock::instance != nullptr)
			{
				TrackClock::instance->reset();
			}
		}
	};
	void fixedUpdate() override {};
};