#pragma once
#include <SFML/Window/Keyboard.hpp>
#include <box2d/box2d.h>


class EndGoal {
public:
	static EndGoal& getInstance() {
		static EndGoal instance;
		return instance;
	}

	// Delete copy/move so it can't be duplicated
	EndGoal(const EndGoal&) = delete;
	EndGoal& operator=(const EndGoal&) = delete;

	void setEndGoal(b2BodyId endGoalBodyId) {_endid = endGoalBodyId;}
	void setPlayer(b2BodyId id) { _playerid = id; }

private:
	EndGoal() {}
	b2BodyId _endid = b2_nullBodyId;
	b2BodyId _playerid = b2_nullBodyId;
};