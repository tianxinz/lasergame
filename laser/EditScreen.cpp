#include <SFML\Graphics.hpp>

#include <memory>
#include<fstream>
#include<iostream>

#include "EditScreen.h"
#include "Game.h"
#include "Grid.h"
#include "Macro.h"
#include "Mirror.h"
#include "Equipment.h"
#include "Target.h"
#include "LevelSelectScreen.h"
#include "MenuScreen.h"
//#include "LevelManagerEdit.h"

void my_callBack_clear_edit()
{
	Game::Screen = std::make_shared<EditScreen>();
}

void my_callBack_save()
{
	
}

void my_callBack_goBack_edit()
{
	Game::Screen = std::make_shared<MenuScreen>();
}

EditScreen::EditScreen()
	:clearButton_edit("Images/clear_button.png"), saveButton_edit("Images/save_button.png"),goBackButton_edit("Images/go_back.png")
{
	clearButton_edit.callBack = &my_callBack_clear_edit;
	saveButton_edit.callBack = &my_callBack_save;
	goBackButton_edit.callBack = &my_callBack_goBack_edit;

	saveButton_edit.setPosition(400, 520);
	clearButton_edit.setPosition(500,520);
	goBackButton_edit.setPosition(600,520);

	buttonManager_edit.addButton("clear", std::make_shared<UserButton>(clearButton_edit));
	buttonManager_edit.addButton("save", std::make_shared<UserButton>(saveButton_edit));
	buttonManager_edit.addButton("edit", std::make_shared<UserButton>(goBackButton_edit));

	Mirror::loadTexture();
	LaserSource::loadTexture();
	Target::loadTexture();
	Photon::loadTexture("Red_Light.png");
	loadGridEdit();
	loadEquipmentEdit();
//	loadUseEquipmentEdit();
	//LevelManagerEdit *level_manager = LevelManagerEdit::getInstance();
}

void EditScreen::handleInput(sf::RenderWindow& window)
{
	EditScreen::tool_manager_edit.update(window);
	EditScreen::handleLaserEdit();
	buttonManager_edit.update(window);
}

void EditScreen::render(sf::RenderWindow& window)
{
	buttonManager_edit.render(window);
	drawGridEdit(window);
	drawEquitmentEdit(window);
	if(EditScreen::tool_manager_edit.getState() == 1 || EditScreen::tool_manager_edit.getState() == 2)
	{
		window.draw(*(EditScreen::tool_manager_edit.getCopyEquipment()));
	}
	//std::cout<<"laser num "<<tool_manager_edit.my_lasers_.size()<<std::endl;
	drawLaserEdit(window);
}

void EditScreen::update(sf::Time delta)
{

}

void EditScreen::handleLaserEdit()
{
	if(tool_manager_edit.changeIdx >= 0)
	{
		//lightPaths_edit
		for(int i=0; i!=lightPaths_edit.size(); i++)
		{
			std::vector<Photon>::iterator lightPaths_it = lightPaths_edit[i].begin();
			for(; lightPaths_it != lightPaths_edit[i].end(); lightPaths_it++)
			{
				if((*lightPaths_it).getIndex() == tool_manager_edit.changeIdx)
				{
					lightPaths_edit[i].erase(lightPaths_it, lightPaths_edit[i].end());
					break;
				}
			}
		
		}
		calculatePathEdit();
		tool_manager_edit.changeIdx = -1;

	}

}

void EditScreen::loadGridEdit()
{
	EditScreen::myGrid_edit.loadGridEdit();
}

void EditScreen::drawGridEdit(sf::RenderWindow& window)
{
	std::vector<std::vector<sf::Sprite>> gridImage = myGrid_edit.getSprites();
	
	for (int i = 0; i < GRID_HEIGHT; i++) 
	{
		for (int j = 0; j < GRID_WIDTH; j++)
		{
			window.draw(gridImage[i][j]);
		}
	}
}


void EditScreen::loadEquipmentEdit()
{

	std::string* text_equipment;
	text_equipment = loadEquipmentTXT_edit("LevelEdit/level_equipment_all.txt");
	int i=0;
	while(!text_equipment[i].empty())
	{
		switch(text_equipment[i][0])
		{
		case LASER_SOURCE_U_RED:
			{
				if(tool_manager_edit.equipments_.count("laser_source_u_red")==0)
				{
					LaserSource lasersource;
					lasersource.setPosition(700,100);
					tool_manager_edit.equipments_.insert(std::pair<std::string, std::shared_ptr<Equipment>>("laser_source_u_red", std::make_shared<LaserSource>(lasersource)));
					tool_manager_edit.equipments_.at("laser_source_u_red")->setTexture(LaserSource::lTexture);
					i++;
				}
				break;
			}
		case DOOR_U_RED:
			{
				if(tool_manager_edit.equipments_.count("target_u_red")==0)
				{
					Target target;
					target.setPosition(700,150);
					tool_manager_edit.equipments_.insert(std::pair<std::string, std::shared_ptr<Equipment>>("target_u_red", std::make_shared<Target>(target)));
					tool_manager_edit.equipments_.at("target_u_red")->setTexture(Target::tTexture);
					i++;
				}
				break;
			}
		case CAPACITOR:
			{
				break;
			}
		case MIRROR:
			{
				if(tool_manager_edit.equipments_.count("mirror")==0)
				{
					Mirror mirror;
					mirror.setPosition(700,250);
					tool_manager_edit.equipments_.insert(std::pair<std::string, std::shared_ptr<Equipment>>("mirror", std::make_shared<Mirror>(mirror)));
					tool_manager_edit.equipments_.at("mirror")->setTexture(Mirror::mTexture);
					i++;
				}
				break;
			}
		case SPLITTER:
			{
				break;
			}
		case WALL:
			{
				break;
			}
		case SWITCH:
			{
				break;
			}
		case GATE:
			{
				break;
			}
		case FILTER_R:
			{
				break;
			}
		case FILTER_B:
			{
				break;
			}
		case BOMB:
			{
				break;
			}
		case POWERED:
			{
				break;
			}
		default:
			break;

		}
	
	}
		
}


void EditScreen::drawEquitmentEdit(sf::RenderWindow& window)
{
	std::map<std::string, std::shared_ptr<Equipment>>::iterator it = tool_manager_edit.equipments_.begin();
	for(; it!= tool_manager_edit.equipments_.end(); it++)
	{
		window.draw(*((*it).second));
	}

	std::map<int, std::shared_ptr<Equipment>>::iterator it_on_grid = tool_manager_edit.equipments_on_grid_.begin();
	for(; it_on_grid!=tool_manager_edit.equipments_on_grid_.end(); it_on_grid ++)
	{
		int x = (*it_on_grid).first/GRID_WIDTH;
		int y = (*it_on_grid).first - GRID_WIDTH*x;
		((*it_on_grid).second)->setPosition((float)(2*MARGIN+y*(BLOCK_SIZE)), (float)(2*MARGIN+x*(BLOCK_SIZE)));
		window.draw(*((*it_on_grid).second));
	}


}

std::string* loadEquipmentTXT_edit(const char* fileName)
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

void EditScreen::calculatePathEdit()
{
	//std::cout<<"laser dir: "<<tool_manager_edit.my_lasers_[0].<<
	sf::FloatRect windowRect(MARGIN, MARGIN, GRID_WIDTH*(BLOCK_SIZE), GRID_HEIGHT*(BLOCK_SIZE));
	lightPaths_edit.clear();
	if(lightPaths_edit.size() == 0)
	{
		for(int i = 0; i != tool_manager_edit.my_lasers_.size(); i++)
		{
			std::vector<Photon> lightPath;
			lightPath.push_back(tool_manager_edit.my_lasers_[i].getPhoton());
			lightPaths_edit.push_back(lightPath);
		}
	}
	for(int i = 0; i != lightPaths_edit.size(); i++)
	{
		Photon current = lightPaths_edit[i].back();
		while(current.getVelocity() != 0.0 && windowRect.contains(current.getPosition()))
		{
			Photon nextPhoton = current;
			int idx = nextPhoton.getIndex();
			if(tool_manager_edit.equipments_on_grid_.count(idx) > 0)
			{
				tool_manager_edit.equipments_on_grid_[idx]->reaction(nextPhoton, lightPaths_edit);
				lightPaths_edit[i].push_back(nextPhoton);
			}
			else
			{
				nextPhoton.myMove();
				lightPaths_edit[i].push_back(nextPhoton);
			}
			current = nextPhoton;
		}
	}
	
	for(int i = 0; i != lightPaths_edit.size(); i++)
	{
		Photon tail = lightPaths_edit[i].back();
		int tailIdx = tail.getIndex();
		if(tool_manager_edit.equipments_on_grid_.count(tailIdx) > 0)
		{
			tool_manager_edit.equipments_on_grid_[tailIdx]->reaction(tail, lightPaths_edit);
		}
	}
	
}


void EditScreen::drawLaserEdit(sf::RenderWindow& window)
{
	for(int i = 0; i != lightPaths_edit.size(); i++)
	{
		for(int j = 0; j != lightPaths_edit[i].size(); j++)
		{
			window.draw(lightPaths_edit[i][j]);
		}
	}
}

