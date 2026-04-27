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

    void setEndGoal(b2BodyId endGoalBodyId) {
        _id = endGoalBodyId;
    }
private:
    EndGoal(){}
    b2BodyId _id = b2_nullBodyId;
    void onPlayerReachedEnd() {
        // trigger level complete, load next scene, etc.
    }
};