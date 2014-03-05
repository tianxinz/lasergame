#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include <SFML\Graphics.hpp>

#include <memory>
#include <vector>

#include "Photon.h"
#include "Screen.h"
#include "Equipment.h"
#include "ToolManager.h"
#include "Macro.h"
#include "Grid.h"
#include "ButtonManager.h"


extern int curr_level;
extern int currentScore;

class GameScreen : public Screen
{
public:
	GameScreen();

	void handleInput(sf::RenderWindow& window) override;
	void update(sf::Time delta) override;
	void render(sf::RenderWindow& window) override;
	void loadGrid();
	void loadEquipment();
	void handleLaser();
	int allHit;
	int renderCount;


private:
	Grid myGrid;
	void drawGrid(sf::RenderWindow& window);
	void drawEquitment(sf::RenderWindow& window);
	void calculatePath();
	void drawLaser(sf::RenderWindow& window);
	void drawEnd(sf::RenderWindow& window);
	ToolManager tool_manager;
	std::vector<std::vector<Photon>> lightPaths;
	ButtonManager buttonManager_game;
	ButtonManager buttonManager_end;
	UserButton clearButton;
	UserButton goBackButton;
	UserButton nextLevelButton;
	UserButton replayButton;
	sf::Texture zero_star;
	sf::Texture one_star;
	sf::Texture two_star;
	sf::Texture three_star;
	sf::Texture transparent_background;
	sf::Texture congratulation;


};

std::string* loadEquipmentTXT(const char* fileName);

#endif