#include "PlayerData.hpp"

std::vector<std::string>& PlayerData::getLevelVector()
{
    return levelNames;
}

std::vector<long long>& PlayerData::getLevelTimes()
{
    return levelTimes;
}

void PlayerData::readSaveData()
{
    std::ifstream inStream("assets/save.csv");

    std::string line;

    levelNames.clear();
    levelTimes.clear();

    //header
    std::getline(inStream, line);

    //file
    while(std::getline(inStream, line))
    {
        std::string uuid;
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
void PlayerData::writeSaveData()
{
    std::ofstream outStream("assets/save.csv");

    outStream << "uuid, bestTime" << std::endl;

    for (int i = 0; i < levelNames.size(); i++)
    {
        outStream << levelNames[i] << "," << levelTimes[i] << std::endl;
    }

    outStream.close();
}

//the string uuid can be changed to an int easy
void PlayerData::setLevelTime(long long time, std::string curLevel)
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

void PlayerData::deleteSaveData()
{
    for(int i = 0; i < levelNames.size(); i++)
    {
        levelTimes[i] = 0;
    }
    writeSaveData();
}

long long PlayerData::getLevelTime(const std::string& levelName) const
{
    for (int i = 0; i < levelNames.size(); i++)
    {
        if (levelName == levelNames[i])
        {
            return levelTimes[i];
        }
    }
    return 0;
}
