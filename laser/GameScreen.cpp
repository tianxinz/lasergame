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

	Mirror::loadTexture();
	LaserSource::loadTexture();
	Target::loadTexture();
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
	if( GameScreen::tool_manager.getState() == 1 || GameScreen::tool_manager.getState() == 2)
	{
		window.draw(*(GameScreen::tool_manager.getCopyEquipment()));
	}
	/*
	//drawPhoton(window)
	//{
	sf::FloatRect windowRect(MARGIN, MARGIN, GRID_WIDTH*(BLOCK_SIZE), GRID_HEIGHT*(BLOCK_SIZE));
		std::vector<Photon> myPhotonVec;
		myPhotonVec.clear();
		for(int i=0; i!=tool_manager.my_Lasers_.size(); i++)
		{
			myPhotonVec.push_back(tool_manager.my_Lasers_[i].getPhoton(2));
		}
		while(myPhotonVec.size() > 0)
		{
			Photon myPhoton = myPhotonVec.back();
			myPhoton.setVelocity(1);
			myPhotonVec.pop_back();
			window.draw(myPhoton);
			while(true) {
				myPhoton.move();
				std::map<int, std::shared_ptr<Equipment>>::iterator it_on_grid = tool_manager.equipments_on_grid_.begin();
				for(; it_on_grid!=tool_manager.equipments_on_grid_.end(); it_on_grid ++)
				{
					if(myPhoton.getPosition() == (*it_on_grid).second->getPosition()) {
						(*it_on_grid).second->reaction(myPhoton);
						break;
					}
				}

				if(myPhoton.getVelocity() == 0.0 || !windowRect.contains(myPhoton.getPosition()))
				{
					break;
				}
				window.draw(myPhoton);
			}
		//}
	
	}*/

}

void GameScreen::update(sf::Time delta)
{

}



void GameScreen::drawGrid(sf::RenderWindow& window)
{
	Grid myGrid;
	std::string* text;
	text = loadTXT("Level/level_1.txt");
	myGrid.loadGrid(text,GameScreen::tool_manager.equipments_on_grid_, GameScreen::tool_manager.my_Lasers_);
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
						target.setPosition(750,200);
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