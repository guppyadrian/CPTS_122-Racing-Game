#pragma once

#include <SFML/System/Clock.hpp>
#include <fstream>


class PlayerDat
{
    public:
    PlayerDat() = default;
    ~PlayerDat() = default;

    //reads save file
    void readSaveData();

    //writes save file (id run this after read ngl just incase so the save doesn't wipe)
    void writeSaveData();

    //curLevel == uuid
    //the string uuid can be changed to an int eas
    void setLevelTime(int time, std::string curLevel);
    private:
    std::vector<double> levelTimes;
    std::vector<std::string> levelNames;

};



