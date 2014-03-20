#include <SFML\Graphics.hpp>

#include <memory>
#include<fstream>
#include<iostream>

#include "GameScreen.h"
#include "Game.h"
#include "Grid.h"
#include "Macro.h"
#include "Mirror.h"
#include "Equipment.h"
#include "Target.h"
#include "LevelSelectScreen.h"
#include "LevelManager.h"
#include "MenuScreen.h"
#include "Splitter.h"


int curr_level = 1;
int currentScore  = 0;
std::string user_curr_level = "";

void my_callBack_goBack()
{
	Game::Screen = std::make_shared<LevelSelectScreen>();
}


void my_callBack_clear()
{

	Game::Screen = std::make_shared<GameScreen>();
}

void my_callBack_next()
{
	curr_level++;
	Game::Screen = std::make_shared<GameScreen>();
}

void my_callBack_replay()
{
	//curr_level--;
	Game::Screen = std::make_shared<GameScreen>();
}

GameScreen::GameScreen()
	:goBackButton("Images/go_back.png"), clearButton("Images/clear_button.png"),nextLevelButton("Images/next_level_button.png"),replayButton("Images/replay_button.png")
{
	//put all the equipments needed in this level.
	goBackButton.callBack = &my_callBack_goBack;
	clearButton.callBack = &my_callBack_clear;
	nextLevelButton.callBack = &my_callBack_next;
	replayButton.callBack = &my_callBack_replay;

	goBackButton.setPosition(300, 520);
	clearButton.setPosition(500, 520);
	buttonManager_game.addButton("goBack", std::make_shared<UserButton>(goBackButton));
	buttonManager_game.addButton("clear", std::make_shared<UserButton>(clearButton));

	nextLevelButton.setPosition(300,400);
	replayButton.setPosition(500,400);
	if(curr_level < LEVEL_NUMBER)
	{
		buttonManager_end.addButton("next_level",std::make_shared<UserButton> (nextLevelButton));
	}
	buttonManager_end.addButton("replay",std::make_shared<UserButton> (replayButton));

	Splitter::loadTexture();
	Mirror::loadTexture();
	LaserSource::loadTexture();
	Target::loadTexture();
	Photon::loadTexture("Red_Light.png");
	loadGrid();
	loadEquipment();
	allHit = 0;
	renderCount = 0;
	LevelManager * level_manager = LevelManager::getInstance();
	std::string levelKey = "";
	if(load_mode == 0)
	{
		char numStr[10] = {};
		itoa(curr_level, numStr, 10);
		std::string num = std::string(numStr);
		levelKey = "level";
		levelKey += num;
	}
	else
	{
		levelKey = "userlevel" + user_curr_level;
	}
	currentScore = level_manager->levelMap[levelKey].getInitialScore();
	if(!background.loadFromFile("Background/GameScreen.jpg"))
	{
		std::cout<< "load game screen background fail!"	<<std::endl;
	}
	backgroundSp.setTexture(background);
}

void GameScreen::handleInput(sf::RenderWindow& window)
{
	GameScreen::tool_manager.update(window);
	GameScreen::handleLaser();
	buttonManager_game.update(window);
	if(allHit == 1)
	{
		//std::cout<<"come into reset gamescreen"<<std::endl;
		renderCount++;
		//std::cout<<"come into reset gamescreen: "<<renderCount<<std::endl;
		if(renderCount == 1)
		{
			LevelManager *level_manager = LevelManager::getInstance();
			std::map<int, std::shared_ptr<Equipment>> :: iterator it = GameScreen::tool_manager.equipments_on_grid_move_.begin();
			for(; it != GameScreen::tool_manager.equipments_on_grid_move_.end(); it++)
			{
				currentScore -= it->second->cost;
			}
			level_manager->saveLevelInfo(curr_level-1, currentScore);
		}
		if(renderCount > 150)
		{
			buttonManager_end.update(window);
		}
	}
}

void GameScreen::render(sf::RenderWindow& window)
{
	window.draw(backgroundSp);
	buttonManager_game.render(window);
	drawGrid(window);
	drawEquitment(window);
	if( GameScreen::tool_manager.getState() == 1 || GameScreen::tool_manager.getState() == 2)
	{
		window.draw(*(GameScreen::tool_manager.getCopyEquipment()));
	}

	drawLaser(window);
	if(allHit == 1)
	{
		if(renderCount > 150)
			drawEnd(window);
	}
	
}

void GameScreen::update(sf::Time delta)
{

}

void GameScreen::handleLaser()
{
	// if the changed equipment is on the previous light path, from that position on, the following photon sprite will be erased
	// otherwise no photon will be removed
	/*
	if(tool_manager.changeIdx >= 0)
	{	
		
		for(int i = 0; i != lightPaths.size(); i++)
		{	
			std::vector<Photon>::iterator lightPaths_it = lightPaths[i].begin();
			for(; lightPaths_it != lightPaths[i].end(); lightPaths_it++)
			{
				if ((*lightPaths_it).getIndex() == tool_manager.changeIdx)
				{
					lightPaths[i].erase(lightPaths_it+1, lightPaths[i].end());
					break;
				}
			}
		}
		
		 calculate the light paths from the last photon in each light path
		calculatePath();
		tool_manager.changeIdx = -1;
	}
	*/
	calculatePath();
}

void GameScreen::loadGrid()
{
	std::string* text;
	std::string level_name;
	if(load_mode == 0)
	{
		level_name = "Level/level_";

		char intStr[10] = {};
		itoa(curr_level, intStr,10);
		std::string str = std::string(intStr);

		level_name += str;
		level_name += ".txt";
	}

	if(load_mode == 1)
	{
		level_name = "UserLevel/" + user_curr_level;
		level_name += ".txt";

	}

	const char* txt_name = level_name.c_str();

	//std::cout<<txt_name<<std::endl;
	text = loadTXT(txt_name);
	GameScreen::myGrid.loadGrid(text,GameScreen::tool_manager.equipments_on_grid_, GameScreen::tool_manager.my_lasers_, GameScreen::tool_manager.my_targets_);
}


void GameScreen::drawGrid(sf::RenderWindow& window)
{
	std::vector<std::vector<sf::Sprite>> gridImage = myGrid.getSprites();
	
	for (int i = 0; i < GRID_HEIGHT; i++) 
		{
			for (int j = 0; j < GRID_WIDTH; j++)
			{
				window.draw(gridImage[i][j]);
			}
		}
}

void GameScreen::loadEquipment()
{
	std::string* text_equipment;
	std::string level_name; 
	if(load_mode == 0)
	{
		level_name	= "Level/level_equipment_";

		char intStr[10] = {};
		itoa(curr_level, intStr,10);
		std::string str = std::string(intStr);

		level_name += str;
		level_name += ".txt";
	}

	if(load_mode == 1)
	{
		level_name = "UserLevel/"+ user_curr_level;
		level_name += "_equipment.txt";

	}

	const char* txt_name = level_name.c_str();

	text_equipment = loadEquipmentTXT(txt_name);
	int i=0;

	while(!text_equipment[i].empty())
	{
		switch( text_equipment[i][0] )
		{
		    case MIRROR : 
				{

					if(tool_manager.equipments_.count("mirror")==0)
					{
						Mirror mirror;
						mirror.setPosition(700,100);
						tool_manager.equipments_.insert(std::pair<std::string, std::shared_ptr<Equipment>>("mirror", std::make_shared<Mirror>(mirror)));
						tool_manager.equipments_.at("mirror")->setTexture(Mirror::mTexture);
						i++;
					}
					break;
				}
			case SPLITTER :
				{

					if(tool_manager.equipments_.count("splitter")==0)
					{
						Splitter splitter;
						splitter.setPosition(700,200);
						tool_manager.equipments_.insert(std::pair<std::string, std::shared_ptr<Equipment>>("splitter", std::make_shared<Splitter>(splitter)));
						tool_manager.equipments_.at("splitter")->setTexture(Splitter::sTexture);
						i++;
					}
					break;
				}
			default:
				break;
		}
	}
}

void GameScreen::drawEquitment(sf::RenderWindow& window)
{	
		std::map<std::string, std::shared_ptr<Equipment>>::iterator it = tool_manager.equipments_.begin();
		for(; it!= tool_manager.equipments_.end(); it++)
		{
			window.draw(*((*it).second));
		}

		std::map<int, std::shared_ptr<Equipment>>::iterator it_on_grid = tool_manager.equipments_on_grid_.begin();
		for(; it_on_grid!=tool_manager.equipments_on_grid_.end(); it_on_grid ++)
		{
			int x = (*it_on_grid).first/GRID_WIDTH;
			int y = (*it_on_grid).first - GRID_WIDTH*x;
			((*it_on_grid).second)->setPosition((float)(2*MARGIN+y*(BLOCK_SIZE)), (float)(2*MARGIN+x*(BLOCK_SIZE)));
			window.draw(*((*it_on_grid).second));
		}
}

std::string* loadEquipmentTXT(const char* fileName)
{
	std::string* text;
	text = new std::string[10];
	std::ifstream myfile(fileName);
	int lineNum = 0;
	while(std::getline(myfile, text[lineNum]))
	{
		lineNum++;
	}
	return text;
}

void GameScreen::calculatePath()
{ 
	lightPaths.clear();
	for(int i = 0; i != GameScreen::tool_manager.my_targets_.size(); i++)
	{
		GameScreen::tool_manager.my_targets_[i]->lightOff();
	}
	sf::FloatRect windowRect(MARGIN, MARGIN, GRID_WIDTH*(BLOCK_SIZE), GRID_HEIGHT*(BLOCK_SIZE));
	if(lightPaths.size() == 0)
	{
		for(int i = 0; i != tool_manager.my_lasers_.size(); i++)
		{
			std::vector<Photon> lightPath;
			lightPath.push_back(tool_manager.my_lasers_[i].getPhoton());
			lightPaths.push_back(lightPath);
		}
	}
	for(int i = 0; i != lightPaths.size(); i++)
	{
		Photon current = lightPaths[i].back();
		while(current.getVelocity() != 0.0 && windowRect.contains(current.getPosition()))
		{
			Photon nextPhoton = current;
			int idx = nextPhoton.getIndex();
			if(tool_manager.equipments_on_grid_.count(idx) > 0)
			{
				tool_manager.equipments_on_grid_[idx]->reaction(nextPhoton, lightPaths);
				lightPaths[i].push_back(nextPhoton);
			}
			else
			{
				nextPhoton.myMove();
				lightPaths[i].push_back(nextPhoton);
			}
			current = nextPhoton;
		}
	}
	
	for(int i = 0; i != lightPaths.size(); i++)
	{
		Photon tail = lightPaths[i].back();
		int tailIdx = tail.getIndex();
		if(tool_manager.equipments_on_grid_.count(tailIdx) > 0)
		{
			tool_manager.equipments_on_grid_[tailIdx]->reaction(tail, lightPaths);
		}
	}
	
	bool isAllHit = true;
	for(int i = 0; i != GameScreen::tool_manager.my_targets_.size(); i++)
	{
		if(!GameScreen::tool_manager.my_targets_[i]->isHit())
		{
			isAllHit = false;
			break;
		}
	}
	if(isAllHit)
	{
		//std::cout<<"come into isAllHit"<<std::endl;
		allHit = 1;
		//curr_level ++;
	}
}

void GameScreen::drawLaser(sf::RenderWindow& window)
{
	for(int i = 0; i != lightPaths.size(); i++)
	{
		for(int j = 0; j != lightPaths[i].size(); j++)
		{
			window.draw(lightPaths[i][j]);
		}
	}
}

void GameScreen::drawEnd(sf::RenderWindow& window)	
{
	//:nextLevelButton("Images/next_level_button.png"), replayButton("Images/replay_button.png")
    //sf::Texture zero_star;
	//sf::Texture one_star;
	//sf::Texture two_star;
	//sf::Texture three_star;
	transparent_background.loadFromFile("Background/transparent_background.png");
	sf::Sprite trans_back;
	trans_back.setTexture(transparent_background);
	sf::Color color(255,255,255,210);
	trans_back.setColor(color);
	window.draw(trans_back);
	congratulation.loadFromFile("Background/congratulation.png");
	sf::Sprite cong;
	cong.setTexture(congratulation);
	cong.setPosition(200,50);
	window.draw(cong);
	buttonManager_end.render(window);

}
