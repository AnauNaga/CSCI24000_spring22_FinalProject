#include "Button.h"
#include "BMPIN.h"
#include "BMPOUT.h"

Button::Button() {
	range.xMin = 0;
	range.yMin = 0;
	range.xMax = 0;
	range.yMax = 0;
	range.width = 0;
	range.height = 0;
	buffer_buttonUp = malloc(5424 * 4);
	buffer_buttonDown = malloc(5424 * 4);
}
Button::Button(int x,int y, std::string text, unsigned int* parentBuffer, int buffer_width){
	Button::text = text;
 	int addon = 0;
	if (text.length() > 4) { addon = 24 * (text.length() - 4); }
	range.xMin = x;
	range.yMin = y;
	range.xMax = x + addon + 113;
	range.yMax = y + 48;
	range.width = range.xMax - range.xMin;
	range.height = range.yMax - range.yMin;
	Button::buffer_buttonUp = malloc(range.width * range.height * 4);
	Button::buffer_buttonDown = malloc(range.width * range.height * 4);
	is_down = false;
	changed = false;
	BMPIN* buttonUp = new BMPIN();
	buttonUp->Import("text/ButtonUp.bmp", 0x00FFFFFF);
	BMPIN* buttonDown = new BMPIN();
	buttonDown->Import("text/ButtonDown.bmp", 0x00FFFFFF);
	
	unsigned int* pixel = (unsigned int*)buffer_buttonUp;
	Button::parentBuffer = parentBuffer;

	//save the buttonUp image into memory
	unsigned int currColor;
	for (int bufferY = 0; bufferY < 48; bufferY++) {
		for (int bufferX = 0; bufferX < range.width; bufferX++) {

			//0xFF000000 means transparent. All pixels are transparent until said otherwise.
			*pixel = 0xFF000000;

			
			if (bufferX < (range.width / 2) - addon/2) {
				if (buttonUp->getColorHex(bufferX, bufferY) != 0x00FFFFFF) {*pixel = buttonUp->getColorHex(bufferX, bufferY);}
			}
			else if (bufferX < range.width /2 + addon/2) {
				if (buttonUp->getColorHex(6, bufferY) != 0x00FFFFFF) { *pixel = buttonUp->getColorHex(6, bufferY); }
			}
			else {
				if (buttonUp->getColorHex(bufferX - addon, bufferY) != 0x00FFFFFF) { *pixel = buttonUp->getColorHex(bufferX - addon, bufferY); }
			}
			pixel++;

		}
	}
	int offset = 7;

	//save the button text on top of the buttonUp image we save
	pixel = (unsigned int*)buffer_buttonUp;
	char fileName[] = { "text/ .BMP" };
	for (int currChar = 0; currChar < text.length(); currChar++) {
		BMPIN* letter = new BMPIN();
		fileName[5] = text[currChar];
		if (text[currChar] == (char)":") {
			
		}
		if (letter->Import(fileName) == 0) {
			for (int bufferY = 0; bufferY < 29; bufferY++) {
				for (int bufferX = 0; bufferX < 24; bufferX++) {
					unsigned int* pixel = (unsigned int*)buffer_buttonUp + bufferX + offset + ((bufferY + 11) * range.width);
					*pixel = letter->getColorHex(bufferX, bufferY);
				}
			}
		}
		delete letter;
		offset += 24;
	}
	delete buttonUp;

	//save the buttonDown image into memory
	pixel = (unsigned int*)buffer_buttonDown;
	for (int bufferY = 0; bufferY < 48; bufferY++) {
		for (int bufferX = 0; bufferX < range.width; bufferX++) {

			//0xFF000000 means transparent. All pixels are transparent until said otherwise.
			*pixel = 0xFF000000;


			//save a pixel if it is NOT whitespace
			
				if (bufferX < (range.width / 2) - addon/2) {
					if (buttonDown->getColorHex(bufferX, bufferY) != 0x00FFFFFF) { *pixel = buttonDown->getColorHex(bufferX, bufferY); }
				}
				else if (bufferX < range.width / 2 + addon/2) {
					if (buttonDown->getColorHex(9, bufferY) != 0x00FFFFFF) { *pixel = buttonDown->getColorHex(9, bufferY); }
				}
				else {
					if (buttonDown->getColorHex(bufferX - addon, bufferY) != 0x00FFFFFF) { *pixel = buttonDown->getColorHex(bufferX - addon, bufferY); }
				}// end if
			pixel++;

		}//end for
	}//end for

	//save the button text on top of the buttonDown image
	offset = 10;
	pixel = (unsigned int*)buffer_buttonDown;

	for (int currChar = 0; currChar < text.length(); currChar++) {
		BMPIN* letter = new BMPIN();
		fileName[5] = text[currChar];
		if (letter->Import(fileName) == 0) {
			for (int bufferY = 0; bufferY < 29; bufferY++) {
				for (int bufferX = 0; bufferX < 24; bufferX++) {
					unsigned int* pixel = (unsigned int*)buffer_buttonDown + bufferX + offset + ((bufferY + 9) * range.width);
					*pixel = letter->getColorHex(bufferX, bufferY);
				}
			}
		}
		offset += 24;
		delete letter;
	}
	delete buttonDown;
	releaseButton(parentBuffer);
}

Button::~Button(){
}

void Button::pressButton(unsigned int* buffer){
	unsigned int* pixel = buffer;
	unsigned int* localpixel = (unsigned int*)Button::buffer_buttonDown;

	for (int bufferY = 0; bufferY < 48; bufferY++) {
		for (int bufferX = 0; bufferX < range.width; bufferX++) {
			if ((*localpixel >> 24) == 00) {
				pixel = buffer + bufferX + range.xMin + ((bufferY + range.yMin) * 1920);
				*pixel = *localpixel;
			}
			localpixel++;
		}
	}

}

void Button::releaseButton(unsigned int* buffer){
	unsigned int* pixel = buffer;
	unsigned int* localpixel = (unsigned int*)buffer_buttonUp;

	for (int bufferY = 0; bufferY < 48; bufferY++) {
		for (int bufferX = 0; bufferX < range.width; bufferX++) {
			if ((*localpixel >> 24) == 00) {
				pixel = buffer + bufferX + range.xMin + ((bufferY + range.yMin) * 1920);
				*pixel = *localpixel;
			}
			localpixel++;
		}
	}

}

Range Button::getRange(){
	return range;
}

std::string Button::getText(){
	return text;
}

void Button::setText(){
}
