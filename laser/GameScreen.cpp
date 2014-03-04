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




int curr_level = 1;
void my_callBack_goBack()
{
	Game::Screen = std::make_shared<LevelSelectScreen>();
}


void my_callBack_clear()
{

	Game::Screen = std::make_shared<GameScreen>();
}

GameScreen::GameScreen()
	:goBackButton("Images/go_back.png"), clearButton("Images/clear_button.png")
{
	//put all the equipments needed in this level.
	goBackButton.callBack = &my_callBack_goBack;
	clearButton.callBack = &my_callBack_clear;

	goBackButton.setPosition(300, 520);
	clearButton.setPosition(500, 520);
	buttonManager_game.addButton("goBack", std::make_shared<UserButton>(goBackButton));
	buttonManager_game.addButton("clear", std::make_shared<UserButton>(clearButton));


	Mirror::loadTexture();
	LaserSource::loadTexture();
	Target::loadTexture();
	Photon::loadTexture("Red_Light.png");
	loadGrid();
	loadEquipment();
	allHit = 0;
}

void GameScreen::handleInput(sf::RenderWindow& window)
{
	//handle the 
	GameScreen::tool_manager.update(window);
	GameScreen::handleLaser();
	buttonManager_game.update(window);
	//std::cout<<allHit<<std::endl;
	if(allHit == 1)
	{
		//std::cout<<"he"<<std::endl;
		Game::Screen = std::make_shared<LevelSelectScreen>();
	}
}

void GameScreen::render(sf::RenderWindow& window)
{
	//sf::Clock clock;
	//sf::Time time_1;
	//sf::Time time_2;
	buttonManager_game.render(window);
	drawGrid(window);
	drawEquitment(window);
	if( GameScreen::tool_manager.getState() == 1 || GameScreen::tool_manager.getState() == 2)
	{
		window.draw(*(GameScreen::tool_manager.getCopyEquipment()));
	}

	drawLaser(window);

}

void GameScreen::update(sf::Time delta)
{

}

void GameScreen::handleLaser()
{
	// if the changed equipment is on the previous light path, from that position on, the following photon sprite will be erased
	// otherwise no photon will be removed
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
		
		// calculate the light paths from the last photon in each light path
		calculatePath();
		tool_manager.changeIdx = -1;
	}
}

void GameScreen::loadGrid()
{
	//std::cout<<"loadGrid:"<<curr_level<<std::endl;
	std::string* text;
	std::string level_name = "Level/level_";

	char intStr[10] = {};
	itoa(curr_level, intStr,10);
	std::string str = std::string(intStr);

	level_name += str;
	level_name += ".txt";

	const char* txt_name = level_name.c_str();


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
	std::string level_name = "Level/level_equipment_";

	char intStr[10] = {};
	itoa(curr_level, intStr,10);
	std::string str = std::string(intStr);

	level_name += str;
	level_name += ".txt";

	const char* txt_name = level_name.c_str();

	text_equipment = loadEquipmentTXT("Level/level_equipment_1.txt");
	int i=0;

	while(!text_equipment[i].empty())
	{
		switch( text_equipment[i][0] )
		{
		    case '1' : 
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
			case '2' :
				{

					if(tool_manager.equipments_.count("target")==0)
					{
						Target target;
						target.setPosition(700,200);
						tool_manager.equipments_.insert(std::pair<std::string, std::shared_ptr<Equipment>>("target", std::make_shared<Target>(target)));
						tool_manager.equipments_.at("target")->setTexture(Target::tTexture);
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
		bool isHit = false;
		if(!GameScreen::tool_manager.my_targets_[0]->isHit())
		{
			isAllHit = false;
			break;
		}
	}
	if(isAllHit)
	{
		allHit = 1;
		curr_level ++;
		if(curr_level == 3)
			curr_level = 1;
		//Game::Screen = std::make_shared<GameScreen>();


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
