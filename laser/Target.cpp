#include "Target.h"
#include "Photon.h"
#include <string.h>
#include <iostream>
#include <cmath>
#include <SFML\Graphics.hpp>

sf::Texture Target::tTexture;
sf::Texture Target::tHitTexture;
Target::Target()
{
	col = sf::Color::Red;
	isHit = false;
	setTexture(Target::tTexture);

}

void Target::reaction(Photon& photon)
{
	photon.setVelocity(0.0);
	isHit = true;
	setTexture(Target::tHitTexture);
}

bool Target::isSuccess()
{
	return isHit;
}
void Target::setColor(sf::Color myCol)
{
	col = myCol;
}
void Target::loadTexture()
{
	if(!(Target::tTexture.loadFromFile("Equipments_Image/Target.png")))
	{
		std::cout << "Error: could not load Target image!" << std::endl;
	}
	if(!Target::tHitTexture.loadFromFile("Equipments_Image/Target_Hit.png"))
	{
		std::cout << "Error: could not load Laser Source image!" << std::endl;
	}
}
void Target::clone(std::shared_ptr<Equipment>& ePtr){}
void Target::myRotate(){}