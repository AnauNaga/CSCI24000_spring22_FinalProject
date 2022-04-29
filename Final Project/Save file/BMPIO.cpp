#include "BMPIO.h"
#include <iostream>
#include <fstream>


BMPIO::BMPIO(){
}

BMPIO::~BMPIO()
{

}

unsigned int BMPIO::getColorHex(int x, int y) const{
	return *((unsigned int*)image_buffer + x + image_width * y);
}

void BMPIO::setColor(unsigned int color, int x, int y){
	*((unsigned int*)image_buffer + x + y * image_width)  = color;
}

int BMPIO::getHeight(){
	return image_height;
}

int BMPIO::getWidth(){
	return image_width;
}


