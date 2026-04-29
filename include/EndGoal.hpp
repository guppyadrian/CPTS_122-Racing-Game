#pragma once
#include <vector>
#include <string>
#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>
#include "TrackClock.hpp"

class EndGoal {
public:
	static EndGoal& getInstance() {
		static EndGoal instance;
		return instance;
	}

	EndGoal(const EndGoal&) = delete;
	EndGoal& operator=(const EndGoal&) = delete;

	void setEndGoal(b2BodyId endGoalBodyId) { _endid = endGoalBodyId; }
	void setPlayer(b2BodyId id) { _playerid = id; }

	int getLaps() { return _lapsLeft; }
	void setLaps(int laps) { _lapsLeft = laps; _lapsOG = laps; }
	bool lapComplete()
	{
		if (_lapsLeft <= 1) return true;
		_lapsLeft--;

		//Player reset
		float radians = playerStartRot * (B2_PI / 180.0f);
		b2Rot myRotation;
		myRotation.c = b2ComputeCosSin(radians).cosine;
		myRotation.s = b2ComputeCosSin(radians).sine;
		b2Body_SetTransform(_playerid, { playerStartPos.x , playerStartPos.y }, myRotation);
		b2Body_SetAngularVelocity(_playerid, 0.f);
		b2Body_SetLinearVelocity(_playerid, b2RotateVector(myRotation, { 0.f,-100.f }));

		//Timer reset
		trackClockRef->lapDone();
		_collide = false;

		return false;
	}
	bool finished()
	{
		if (_lapsLeft == 1 && _collide)
		{
			_lapsLeft = 0;
			trackClockRef->lapDone();

			float radians = playerStartRot * (B2_PI / 180.0f);

			b2Rot myRotation;
			myRotation.c = b2ComputeCosSin(radians).cosine;
			myRotation.s = b2ComputeCosSin(radians).sine;
			b2Body_SetTransform(_playerid, { playerStartPos.x , playerStartPos.y }, myRotation);
			b2Body_SetAngularVelocity(_playerid, 0.f);
			b2Body_SetLinearVelocity(_playerid, { 0.f,0.f });

			reset();
			return true;
		}
		return false;
	}

	void update()
	{
		if (_endTimeout.getElapsedTime().asMilliseconds() < 500) return;
		//EndGoal Update
		if (!b2Body_IsValid(_endid) || !b2Body_IsValid(_playerid)) return;

		int capacity = b2Body_GetContactCapacity(_endid);
		if (capacity == 0) return;

		std::vector<b2ContactData> contacts(capacity);
		int count = b2Body_GetContactData(_endid, contacts.data(), capacity);

		for (int i = 0; i < count; ++i)
		{
			b2BodyId bodyA = b2Shape_GetBody(contacts[i].shapeIdA);
			b2BodyId bodyB = b2Shape_GetBody(contacts[i].shapeIdB);

			if (B2_ID_EQUALS(bodyA, _playerid) || B2_ID_EQUALS(bodyB, _playerid))
			{
				_collide = true;
				lapComplete();
				_endTimeout.restart();
			}
		}
	}

	void reset()
	{
		_lapsLeft = _lapsOG;
		_collide = false;
		//Clock Update
		trackClockRef->_lapsLeft = _lapsOG;
		trackClockRef->_lapsOG = _lapsOG;
	}

	sf::Vector2f playerStartPos = {0.f,0.f};
	float playerStartRot = 0;

	TrackClock* trackClockRef;

private:
	EndGoal()
	{
		playerStartRot = 0;
		_endTimeout.start();
	}
	b2BodyId _endid = b2_nullBodyId;
	b2BodyId _playerid = b2_nullBodyId;
	bool _collide = false;
	int _lapsLeft = 1;
	int _lapsOG = 1;
	bool _lCb = false;
	sf::Clock _endTimeout;
};