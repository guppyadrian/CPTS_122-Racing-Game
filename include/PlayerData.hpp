#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>


class PlayerData
{
    public:
    static PlayerData& getInstance() {
        static PlayerData instance;
        return instance;
    }

    PlayerData(const PlayerData&) = delete;
    PlayerData& operator=(const PlayerData&) = delete;

    std::vector<std::string>& getLevelVector();

    //literally only needed for the tests
    std::vector<long long>& getLevelTimes();

    //reads save file
    void readSaveData();

    //writes save file (id run this after read ngl just incase so the save doesn't wipe)
    void writeSaveData();

    void deleteSaveData();

    long long getLevelTime(const std::string& levelName) const;

    //curLevel == uuid
    //the string uuid can be changed to an int eas
    void setLevelTime(long long time, std::string curLevel);
    private:
    PlayerData() = default;
    std::vector<long long> levelTimes;
    std::vector<std::string> levelNames;

};



