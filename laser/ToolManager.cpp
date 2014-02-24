#include <SFML/Graphics.hpp>

#include <string>
#include <iostream>
#include <map>

#include "ToolManager.h"

ToolManager::ToolManager()
{
	mouseBounds_.width = 1.f;
	mouseBounds_.height = 1.f;
	state = 0;
}

void ToolManager::update(sf::RenderWindow& window)
{
	
	mouseBounds_.left = (float)sf::Mouse::getPosition(window).x;
	mouseBounds_.top = (float)sf::Mouse::getPosition(window).y;

	if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		std::map<std::string, std::shared_ptr<Equipment>>::iterator it = equipments_.begin();
		for(; it!= equipments_.end(); it++)
		{
			if((*it).second->getGlobalBounds().intersects(mouseBounds_) || ToolManager::getState() == 1)
			{
							
				if(ToolManager::getState() == 0)
				{

					std::shared_ptr<Equipment> new_equipment;
					((*it).second)->clone(new_equipment);
					ToolManager::setCopyEquipment(new_equipment);
					new_equipment->setPosition((sf::Vector2f) sf::Mouse::getPosition(window) );
					ToolManager::setState(1);
				}
				
				if(ToolManager::getState() == 1)
				{
					std::shared_ptr<Equipment> new_equipment = ToolManager::getCopyEquipment();
					new_equipment->setPosition((sf::Vector2f) sf::Mouse::getPosition(window) );

				}


			}
		}
	}

	if( !sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if(ToolManager::state == 1)
		{
			unsigned int x = sf::Mouse::getPosition(window).x;
			unsigned int y = sf::Mouse::getPosition(window).y;
			if(x>20&&x<620&&y>20&&y<500)
			{
				int row = (y-MARGIN)/BLOCK_SIZE;
				int col = (x-MARGIN)/BLOCK_SIZE;
				std::shared_ptr<Equipment> new_equipment;
				(ToolManager::copy_equipment)->clone(new_equipment);
				ToolManager::equipments_on_grid_.insert(std::pair<int, std::shared_ptr<Equipment>>((row*GRID_WIDTH + col), new_equipment));
			}

		}
		ToolManager::setState(0);
					
	}

	if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		std::map<int, std::shared_ptr<Equipment>>::iterator it_on_grid = ToolManager::equipments_on_grid_.begin();
		for(; it_on_grid!=ToolManager::equipments_on_grid_.end(); it_on_grid ++)
		{
			if((*it_on_grid).second->getGlobalBounds().intersects(mouseBounds_))
			{
				(*it_on_grid).second->rotate();
			}

		}
	}


}

// display the equipment, equipment itself can display
void ToolManager::render(sf::RenderWindow& window)
{

}

void ToolManager::addEquipment(const std::string& id, std::shared_ptr<Equipment>& equipment)
{
	equipments_[id] = equipment;
}

void ToolManager::removeEquipment(const std::string& id)
{
	equipments_.erase(id);
}

int ToolManager::getState()
{
	return ToolManager::state;
}

void ToolManager::setState(int val)
{
	ToolManager::state = val;
}

std::shared_ptr<Equipment> ToolManager::getCopyEquipment()
{
	return ToolManager::copy_equipment;
}

void ToolManager::setCopyEquipment(std::shared_ptr<Equipment>& new_copy)
{
	ToolManager::copy_equipment = new_copy;
}