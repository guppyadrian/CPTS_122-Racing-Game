#include "playerDat.h"

std::vector<std::string>& PlayerDat::getLevelVector()
{
    return levelNames;
}

std::vector<double>& PlayerDat::getLevelTimes()
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
        levelTimes.push_back(stod(tok));
    }

    inStream.close();
}

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
void PlayerDat::setLevelTime(double time, std::string curLevel)
{
    int levelNum = 0;

    while (levelNum < levelNames.size() && curLevel != levelNames[levelNum])
    {
        levelNum++;
    }

    if (levelNames[levelNum] == curLevel)
    {
        if (levelTimes[levelNum] < time)
        {
            levelTimes[levelNum] = time;
        }
    }
    else
    {
        //error so the number stays the same (no code needed)
    }
}

void PlayerDat::deleteSaveData()
{
    for(int i = 0; i < levelNames.size(); i++)
    {
        levelTimes[i] = 0;
    }
    writeSaveData();
}