#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>


class PlayerDat
{
    public:
    PlayerDat() = default;
    ~PlayerDat() = default;

    std::vector<std::string>& getLevelVector();

    //literally only needed for the tests
    std::vector<float>& getLevelTimes();

    //reads save file
    void readSaveData();

    //writes save file (id run this after read ngl just incase so the save doesn't wipe)
    void writeSaveData();

    void deleteSaveData();

    //curLevel == uuid
    //the string uuid can be changed to an int eas
    void setLevelTime(float time, std::string curLevel);
    private:
    std::vector<float> levelTimes;
    std::vector<std::string> levelNames;

};



