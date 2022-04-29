#ifndef SPACEOBJECT_H_INCLUDED
#define SPACEOBJECT_H_INCLUDED
#include "framework.h"
#include "BMPIN.h"
#include "CreateNewFile.h"

class SpaceObject {
	SpaceObject(std::string filePath);
	~SpaceObject();
	void updateObject(float delta_time);
	LONG getCurrentStatus(float delta_time, int currPositionX, int currPositionY);
private:
	int currPositionX;
	int currPositionY;
	BMPIN* ImageFile;
	SpaceObject* childrenObjects[];

};

#endif