#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

#include <iostream>
#include <ostream>
#include <map>
#include "LevelInfo.h"
#include "Macro.h"

class LevelManager
{
private:
    static bool instanceFlag;
    static LevelManager *level_manager;
    LevelManager()
	{
		for(int i = 1; i <= 5; i++)
		{
			std::string levelName = "Level/level_";
			char numStr[10] = {};
			itoa(i, numStr, 10);
			std::string num = std::string(numStr);
			std::string levelKey = "level";
			levelKey += num;
			levelName += num;
			levelName += "_info.txt";
			const char * fileName = levelName.c_str();
			LevelInfo level_info(fileName);
			levelMap.insert(std::pair<std::string, LevelInfo>(levelKey, level_info));
		}
	}
public:
	void saveLevelInfo(int levelLabel, int currScore);
	std::map<std::string, LevelInfo> levelMap;
    static LevelManager* getInstance();
    ~LevelManager()
    {
        instanceFlag = false;
    }
};





#endif