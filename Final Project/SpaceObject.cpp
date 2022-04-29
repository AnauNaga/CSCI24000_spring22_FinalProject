#include "SpaceObject.h"

SpaceObject::SpaceObject(std::string filePath) {
	std::ifstream File;
	File.open(filePath);

	std::stringstream sstream;
    std::string data;
	std::string FileName;
	std::string cosmicBodyType;
	std::string cosmicBodyName;
	getline(File, data, ',');
	FileName = data;
	getline(File, data, ',');
	cosmicBodyType = data;
	getline(File, data, ',');
	cosmicBodyName = data;

}

SpaceObject::~SpaceObject()
{
}

void SpaceObject::updateObject(float delta_time){


}
