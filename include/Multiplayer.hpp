//
// Created by guppy on 4/29/26.
//

#pragma once

// storing multiplayer data between scenes
class Multiplayer
{
private:
    Multiplayer() = default;
public:
    static Multiplayer& getInstance() {
        static Multiplayer instance;
        return instance;
    }
    int id{-1};
    bool inMultiplayer{false};
    long long startTime{-1};
};
