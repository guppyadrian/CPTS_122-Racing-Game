#include "playerDat.hpp"

int main(void)
{
    PlayerDat testData;
    double newTime = 3.3;

    testData.readSaveData();

    testData.setLevelTime(newTime, testData.getLevelVector()[0]);

    std::cout << "first level: " << testData.getLevelVector()[0] << "time: " << testData.getLevelTimes()[0] << std::endl;
    std::cout << "should be a real level name then " << newTime << std::endl;

    testData.deleteSaveData();

    std::cout << "first level: " << testData.getLevelVector()[0] << "time: " << testData.getLevelTimes()[0] << std::endl;
    std::cout << "should be a real level name then " << 0 << std::endl;

}