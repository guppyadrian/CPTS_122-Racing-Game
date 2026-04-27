#include "playerDat.h"

void PlayerDat::readSaveData()
{
    std::ifstream inStream("sav.csv", 'r');

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
void writeSaveData()
{
    std::ofstream outStream("sav.csv", 'w');



    outStream.close();
}
//the int can be changed to a string uuid pretty easy
void setLevelTime(sf::Clock time, int curLevel)
{

}