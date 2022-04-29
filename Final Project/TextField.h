#ifndef TEXTFIELD_H_INCLUDED
#define TEXTFIELD_H_INCLUDED
#include <iostream>
#include <stdio.h>	
#include "Button.h";
#include "framework.h"

//local includes
#include "Button.h"
#include "KeyTracker.h"
#include "BMPIO.h"
#include "BMPOUT.h"
#include "BMPIN.h"
#include "GuiUtilites.h"
#include "TextField.h"
#include "FinalProject.h"
#include "DeleteFile.h"
#include "ViewFile.h"
#include "LoadFiles.h"
#include "TextField.h"
#include "SpaceObject.h"
#include "CreateNewFile.h"


class TextField {
public:
	TextField();
	TextField(int x, int y, int maxChars, unsigned int* buffer_interactable, int buffer_width, struct Globals* Global, bufferMem* buffer);
	~TextField();

	void displayTextField(unsigned int* buffer);
	Range getRange();
	void removeLetter();
	void Addletter(char newLetter);
	char* getText();
	void removeEndingSpace();
	POINT getCoords();
	POINT getTextCoords();
	void reset();
	bool is_down;
	bool changed;
	POINT currCoords;
	POINT textCoords;
private:
	unsigned int* parentBuffer;
	void* buffer_textField;
	char* text;
	Range range;
};

#endif

