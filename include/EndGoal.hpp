#pragma once
#include <vector>
#include <SFML/Window/Keyboard.hpp>
#include <box2d/box2d.h>

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

    bool getCollide() { return _collide; }
    void setCollide(bool t) { _collide = t; }

    void update()
    {
        if (!b2Body_IsValid(_endid) || !b2Body_IsValid(_playerid))
            return;

        int capacity = b2Body_GetContactCapacity(_endid);
        if (capacity == 0)
            return;

        std::vector<b2ContactData> contacts(capacity);
        int count = b2Body_GetContactData(_endid, contacts.data(), capacity);

        for (int i = 0; i < count; ++i)
        {
            b2BodyId bodyA = b2Shape_GetBody(contacts[i].shapeIdA);
            b2BodyId bodyB = b2Shape_GetBody(contacts[i].shapeIdB);

            if (B2_ID_EQUALS(bodyA, _playerid) || B2_ID_EQUALS(bodyB, _playerid))
            {
                _collide = true;
                return;
            }
        }
    }

private:
    EndGoal() {}
    b2BodyId _endid = b2_nullBodyId;
    b2BodyId _playerid = b2_nullBodyId;
    bool _collide = false;
};