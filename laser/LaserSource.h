#ifndef LASER_SOURCE_H
#define LASER_SOURCE_H

#include <SFML\Graphics.hpp>
#include "Equipment.h"
#include "Photon.h"
#include <string.h>

class LaserSource : public Equipment
{
	sf::Color col;

public:
	LaserSource();
	void setColor(sf::Color myCol);
	void myRotate() override; // dummy rotation function
	void reaction(Photon &photon) override;
	void clone(std::shared_ptr<Equipment>& ePtr) override; // dummy clone function
	Photon getPhoton(float rad);
	static sf::Texture lTexture;
	static void loadTexture();
};

//LaserSource::eTexture.loadFromFile("LaserSource.png");

#endif