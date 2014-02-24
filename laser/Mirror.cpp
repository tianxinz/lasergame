#include "Mirror.h"
#include "Photon.h"
#include <string.h>
#include <iostream>
#include <cmath>

sf::Texture Mirror::mTexture;
Mirror::Mirror()
{
	setTexture(Mirror::mTexture);
}

void Mirror::reaction(Photon& photon)
{
	float mAngle = getRotation();
	float pAngle = photon.getDirection();
	float pAngle_r = pAngle+180 >= 360 ? pAngle-180 : pAngle+180;
	if(fabs(mAngle - pAngle_r) < 90 || fabs(mAngle - pAngle_r) > 270)
	{
		pAngle = pAngle_r + 2 * (mAngle-pAngle_r);
		if(pAngle < 0) pAngle += 360;
		if(pAngle >= 360) pAngle -= 360;
		photon.setDirection(pAngle);
	}
	else 
	{
		photon.setVelocity(0.0);
	}
}

void Mirror::clone(std::shared_ptr<Equipment>& ePtr)
{
	Mirror copyMirror = *this;
	ePtr = std::make_shared<Mirror>(copyMirror);
	
}

void Mirror::myRotate()
{
	setRotation(this->getRotation()+45);
}

void Mirror::loadTexture()
{
	if(!(Mirror::mTexture.loadFromFile("Equipments_Image/Mirror.png")))
	{
		std::cout << "Error: could not load Mirror image!" << std::endl;
	}

}