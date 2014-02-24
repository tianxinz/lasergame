#include "LaserSource.h"
#include "Photon.h"
#include <string.h>
#include <iostream>
#include <cmath>
#include <SFML\Graphics.hpp>

sf::Texture LaserSource::lTexture;
LaserSource::LaserSource()
{
	col = sf::Color::Red;
	setTexture(LaserSource::lTexture);
}
void LaserSource::reaction(Photon& photon)
{
	photon.setVelocity(0.0);
}

void LaserSource::setColor(sf::Color myCol)
{
	col = myCol;
}

Photon LaserSource::getPhoton(float rad)
{
	float angle = getRotation();
	Photon startP(col, rad, angle);
	startP.setPosition(LaserSource::getPosition());
	return startP;
}

void LaserSource::loadTexture()
{
	if(!(LaserSource::lTexture.loadFromFile("Equipments_Image/LaserSource.png")))
	{
		std::cout << "Error: could not load LaserSource image!" << std::endl;
	}
}
void LaserSource::clone(std::shared_ptr<Equipment>& ePtr){}
void LaserSource::myRotate(){}