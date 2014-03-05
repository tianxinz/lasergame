#include <SFML/Graphics.hpp>

#include <string>
#include <iostream>
#include<map>

#include "ButtonManager.h"
#include "GameScreen.h"


ButtonManager::ButtonManager()
{
	mouseBounds_.width = 1.f;
	mouseBounds_.height = 1.f;
}

void ButtonManager::update(sf::RenderWindow& window)
{
	mouseBounds_.left = (float)(sf::Mouse::getPosition(window).x);
	mouseBounds_.top = (float)(sf::Mouse::getPosition(window).y);

	if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
	    std::map<std::string, std::shared_ptr<UserButton>>::iterator it = ButtonManager::buttons_.begin();
		for(;it!=ButtonManager::buttons_.end(); it++)
		{
			if((*it).second->getBounds().intersects(mouseBounds_))
			{
				if((*it).first.compare(0,5,"level") == 0 && (*it).first[5]!='_')
				{
					std::string s_level = (*it).first.substr(5);
					curr_level = atoi(s_level.c_str());
				}
				(*it).second->callBack();
				break;
			}
		}
		
		

	}
}

void ButtonManager::render(sf::RenderWindow& window)
{
	std::map<std::string, std::shared_ptr<UserButton>>::iterator it = ButtonManager::buttons_.begin();
		for(;it!=ButtonManager::buttons_.end(); it++)
		{
			(*it).second->render(window);
		}
}

void ButtonManager::addButton(const std::string& id, std::shared_ptr<UserButton>& button)
{
	buttons_[id] = button;
}

void ButtonManager::removeButton(const std::string& id)
{
	buttons_.erase(id);
}