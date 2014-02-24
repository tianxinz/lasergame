#ifndef MIRROR_H
#define MIRROR_H

#include <SFML\Graphics.hpp>
#include "Equipment.h"
#include "Photon.h"
#include <string.h>

class Mirror : public Equipment
{
public:
	Mirror();
	void reaction(Photon &photon) override;
	void clone(std::shared_ptr<Equipment>& copy_ptr) override;
};

#endif