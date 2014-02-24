#include <SFML/Graphics.hpp>

#include <string>
#include <iostream>
#include <map>

#include "ToolManager.h"

ToolManager::ToolManager()
{
	mouseBounds_.width = 1.f;
	mouseBounds_.height = 1.f;
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
			if((*it).second->getGlobalBounds().intersects(mouseBounds_))
			{
				//clone a equipment
				//std::shared_ptr<Equipment> new_e;
								
				/*sf::Image img = window.capture();
				sf::Texture curr_texture;
				curr_texture.loadFromImage(img);
				sf::Sprite curr_sprite;
				curr_sprite.setTexture(curr_texture);
				*/

				//std::shared_ptr<Equipment> new_equipment;
				//((*it).second)->clone(new_equipment);
				
				sf::RenderTexture rt;
				rt.create(800,600);

				
				while(sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{

					std::shared_ptr<Equipment> new_equipment;
				    ((*it).second)->clone(new_equipment);
					new_equipment->setPosition((sf::Vector2f) sf::Mouse::getPosition(window) );
					//window.clear();
					//window.draw(curr_sprite);
					//window.draw(*new_equipment);
					//window.display();
					rt.clear();
					rt.draw(*new_equipment);
					//rt.display();
					sf::Texture texture = rt.getTexture();
					sf::Sprite sprite(texture);
					window.draw(sprite);
					//window.display();
					


										
				}
				

				//move

				//while(mouse.isPressed)

				//mouse.isReleased, place the equipment

				//vector.add(equipment)

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