#include "PlayerDat.hpp"

std::vector<std::string>& PlayerDat::getLevelVector()
{
    return levelNames;
}

std::vector<long long>& PlayerDat::getLevelTimes()
{
    return levelTimes;
}

void PlayerDat::readSaveData()
{
    std::ifstream inStream("sav.csv");

    std::string line;

    //header
    std::getline(inStream, line);

    //file
    while(std::getline(inStream, line))
    {
        std::string uuid;
        int time;
        std::stringstream lineStream(line);
        std::string tok;

        std::getline(lineStream, tok, ',');
        levelNames.push_back(tok);

        std::getline(lineStream, tok, '\n');
        levelTimes.push_back(stof(tok));
    }
    writeSaveData();
    inStream.close();
}

//change how far the levelTimes decimal goes out in this print and it will do it through the whole project
void PlayerDat::writeSaveData()
{
    std::ofstream outStream("sav.csv");

    outStream << "uuid, bestTime" << std::endl;

    for (int i = 0; i < levelNames.size(); i++)
    {
        outStream << levelNames[i] << "," << levelTimes[i] << std::endl;
    }

    outStream.close();
}

//the string uuid can be changed to an int easy
void PlayerDat::setLevelTime(long long time, std::string curLevel)
{
    for (int i = 0; i < levelNames.size(); i++)
    {
        if (curLevel == levelNames[i])
        {
            if (levelTimes[i] > time || levelTimes[i] == 0)
                levelTimes[i] = time;
            writeSaveData();
            return;
        }
    }

    levelNames.push_back(curLevel);
    levelTimes.push_back(time);
    writeSaveData();
}

void PlayerDat::deleteSaveData()
{
    for(int i = 0; i < levelNames.size(); i++)
    {
        levelTimes[i] = 0;
    }
    writeSaveData();
}