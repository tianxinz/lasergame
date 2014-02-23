#ifndef BUTTON_MANAGER_H
#define BUTTON_MANAGER_H


#include <SFML/Graphics.hpp>

#include <map>
#include <string>
#include <memory>

#include "UserButton.h"

class ButtonManager
{
public:
	ButtonManager();

	void update(sf::RenderWindow& window);
	void render(sf::RenderWindow& window);

	void addButton(const std::string& id, std::shared_ptr<UserButton>& button);
	void removeButton(const std::string& id);

private:
	sf::FloatRect mouseBounds_;
	std::map<std::string, std::shared_ptr<UserButton>> buttons_;

};


#endif