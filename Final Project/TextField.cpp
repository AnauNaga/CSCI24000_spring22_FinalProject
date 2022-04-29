#include "TextField.h"
#include "BMPIN.h"
TextField::TextField(){
	text = new char[2];
}

TextField::TextField(int x, int y, int maxChars, unsigned int* buffer_interactable, int buffer_width, Globals* Global, bufferMem* buffer){
	text = new char[2];
	text[0] = ' ';
	text[1] = '\0';
	currCoords = { x + 5,y + 12 };
	textCoords = { x + 5,y + 12 };

	int addon = 0;
	if (maxChars > 4) { addon = 24 * (maxChars - 4); }
	range.xMin = x;
	range.yMin = y;
	range.xMax = x + addon + 113;
	range.yMax = y + 48;
	range.width = range.xMax - range.xMin;
	range.height = range.yMax - range.yMin;
	buffer_textField = malloc(range.width * range.height * 4);
	BMPIN* TextField = new BMPIN();
	TextField->Import("text/textField.bmp", 0x00FFFFFF);

	unsigned int* pixel = (unsigned int*)buffer_textField;
	TextField::parentBuffer = parentBuffer;

	//save the textfield image into memory
	unsigned int currColor;
	for (int bufferY = 0; bufferY < 48; bufferY++) {
		for (int bufferX = 0; bufferX < range.width; bufferX++) {

			//0xFF000000 means transparent. All pixels are transparent until said otherwise.
			*pixel = 0xFF000000;


			if (bufferX < (range.width / 2) - addon / 2) {
				if (TextField->getColorHex(bufferX, bufferY) != 0x00FFFFFF) { *pixel = TextField->getColorHex(bufferX, bufferY); }
			}
			else if (bufferX < range.width / 2 + addon / 2) {
				if (TextField->getColorHex(6, bufferY) != 0x00FFFFFF) { *pixel = TextField->getColorHex(6, bufferY); }
			}
			else {
				if (TextField->getColorHex(bufferX - addon, bufferY) != 0x00FFFFFF) { *pixel = TextField->getColorHex(bufferX - addon, bufferY); }
			}
			pixel++;

		}
	}
	delete TextField;
}


TextField::~TextField(){
	delete buffer_textField;
}

void TextField::displayTextField(unsigned int* buffer){
	unsigned int* pixel = buffer;
	unsigned int* localpixel = (unsigned int*)buffer_textField;

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

Range TextField::getRange(){
	return range;
}

void TextField::removeLetter(){
	currCoords.x -= 24;
	char* oldword = text;
	int length = strlen(oldword);
	oldword[length - 1] = '\0';
	text = new char[length];
	strcpy_s(text, length, oldword);
	text[length - 1] = '\0';
	text[length - 2] = ' ';
	delete[] oldword;
}

void TextField::Addletter(char newLetter){
	if (strlen(text) == 0) {
		text[0] = ' ';
		text[1] = '\0';
	
	}

	currCoords.x += 24;
	//reallocate more space for our file name
	char* oldword = text;
	int length = strlen(text) + 2;
	text = new char[length];
	strcpy_s(text, length, oldword);
	delete[] oldword;
	text[length - 1] = '\0';
	text[length - 2] = ' ';
	text[length - 3] = newLetter;
	
}

char* TextField::getText(){
	return text;
}

POINT TextField::getCoords(){
	return currCoords;
}

POINT TextField::getTextCoords(){
	return textCoords;
}


void TextField::reset() {
	TextField::currCoords.y = textCoords.y;
	TextField::currCoords.x = textCoords.x;
	delete text;
	TextField::text = new char[2];
	text[0] = ' ';
	text[1] = '\0';
}

void TextField::removeEndingSpace() {
	if (text[strlen(text) - 1] == ' ') {
		char* newText = new char[strlen(text) + 2];
		for (int i = 0; i < strlen(text) - 1; i++) {
			newText[i] = text[i];
		}
		newText[strlen(text) - 1] = 0;
		delete text;
		TextField::text = newText;
	}
}
