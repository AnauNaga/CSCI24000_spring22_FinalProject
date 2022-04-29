#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED
#include <iostream>
#include <stdio.h>	

struct Range {
	int xMin;
	int xMax;
	int yMin;
	int yMax;
	int width;
	int height;
};


class Button {
public:
	Button();
	Button(int x, int y, std::string text, unsigned int* buffer_interactable,int buffer_width);
	~Button();
	void pressButton(unsigned int* buffer);
	void releaseButton(unsigned int* buffer);
	Range getRange();
	void setText();
	std::string getText();
	bool is_down;
	bool changed;

private:
	std::string text;
	unsigned int* parentBuffer;
	void* buffer_buttonUp;
	void* buffer_buttonDown;
	Range range;
};

#endif
