#ifndef TARGET_H
#define TARGET_H

#include <SFML\Graphics.hpp>
#include "Equipment.h"
#include "Photon.h"
#include <string.h>

class Target : public Equipment
{
	sf::Color col;
	bool isHit;
public:
	Target();
	bool isSuccess();
	void reaction(Photon &photon) override;
	void clone(std::shared_ptr<Equipment>& copy_ptr) override;
	void setColor(sf::Color myCol);

};

#endif