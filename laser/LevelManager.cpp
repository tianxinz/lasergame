#include "LevelManager.h"
#include "LevelInfo.h"

bool LevelManager::instanceFlag = false;
LevelManager* LevelManager::level_manager = NULL;

LevelManager* LevelManager::getInstance()
{
    if(! instanceFlag)
    {
        level_manager = new LevelManager();
        instanceFlag = true;
        return level_manager;
    }
    else
    {
        return level_manager;
    }
}

void LevelManager::saveLevelInfo(int levelLabel, int currScore)
{
	char numStr[10] = {};
	itoa(levelLabel, numStr, 10);
	std::string num = std::string(numStr);
	std::string levelKey = "level";
	levelKey += num;
	levelMap[levelKey].setScore(currScore);
	levelMap[levelKey].saveToFile();
	if(levelLabel < LEVEL_NUMBER) {
		char nextNumStr[10] = {};
		itoa(levelLabel+1, nextNumStr, 10);
		std::string nextNum = std::string(nextNumStr);
		levelKey = "level";
		levelKey += nextNum;
		levelMap[levelKey].unlock();
		levelMap[levelKey].saveToFile();
	}
}