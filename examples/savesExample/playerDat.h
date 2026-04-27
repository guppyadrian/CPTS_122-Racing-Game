#pragma once

#include <SFML/System/Clock.hpp>
#include <fstream>


class PlayerDat
{
    public:
    PlayerDat() = default;
    ~PlayerDat() = default;
    void readSaveData();
    void writeSaveData();
    //the int can be changed to a string uuid pretty easy
    void setLevelTime(sf::Clock time, int curLevel);
    private:
    std::vector<double> levelTimes;
    std::vector<std::string> levelNames;

};



