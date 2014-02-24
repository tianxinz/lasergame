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



GameScreen::GameScreen()
{
	//put all the equipments needed in this level.
	loadEquipment();
}

void GameScreen::handleInput(sf::RenderWindow& window)
{
	//handle the 
	GameScreen::tool_manager.update(window);
}

void GameScreen::render(sf::RenderWindow& window)
{
	drawGrid(window);
	drawEquitment(window);
	if( GameScreen::tool_manager.getState() == 1)
	{
		window.draw(*(GameScreen::tool_manager.getCopyEquipment()));

	}

}

void GameScreen::update(sf::Time delta)
{

}



void GameScreen::drawGrid(sf::RenderWindow& window)
{
	Grid myGrid;
	std::string* text;
	text = loadTXT("Level/level_1.txt");
	myGrid.loadGrid(text);
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
						mirror.setPosition(750,100);
						tool_manager.equipments_.insert(std::pair<std::string, std::shared_ptr<Equipment>>("mirror", std::make_shared<Mirror>(mirror)));
						tool_manager.equipments_.at("mirror")->setTexture(tool_manager.equipments_.at("mirror")->eTexture);
						i++;
					}
					break;
				}
			case '2' :
				{

					if(tool_manager.equipments_.count("target")==0)
					{
						Target target;
						target.setPosition(750,200);
						tool_manager.equipments_.insert(std::pair<std::string, std::shared_ptr<Equipment>>("target", std::make_shared<Target>(target)));
						tool_manager.equipments_.at("target")->setTexture(tool_manager.equipments_.at("target")->eTexture);
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