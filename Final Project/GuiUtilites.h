#ifndef GUIUTILITES_H_INCLUDED
#define GUIUTILITES_H_INCLUDED
#include "framework.h"
#include "BMPIN.h"
#include "BMPOUT.h"
#include <stdio.h>
#include <iostream>
#include "SpaceObject.h"
#include "CreateNewFile.h"

struct bufferMem {
    void* memory = 0;
    void* background = 0;
    void* interactable = 0;
    int width = 0;
    int height = 0;
    BITMAPINFO buffer_bitmap_info;
};

struct timeInfo {
    int frame_rate;
    float timeKeeperAlpha = 0.0f;
    float timeKeeperBeta = 0.0f;
    float timeKeeperCharlie = 0.0f;
    float delta_time = 0.016666f;
    LARGE_INTEGER frame_begin_time;
    LARGE_INTEGER frame_end_time;

    //performance_frequency is the processor clock speed 
    float performance_frequency;
    LARGE_INTEGER perf;

};

struct textLine {
    int x;
    int y;
    char* text;
    textLine* nextNode;
};

int setPixelColor(int X, int Y, unsigned int color, bufferMem* buffer, unsigned int* buffer_memory);
int DrawRect(int X1, int Y1, int X2, int Y2, unsigned int color, bufferMem* buffer, unsigned int* buffer_memory);
int drawLine(struct bufferMem* buffer, int firstX, int firstY, int secondX, int secondY, int thickness);
int setWindowColor(unsigned int color, struct bufferMem* buffer, unsigned int* buffer_memory);
void printText(char textToPrint[], int xStart, int yStart, struct bufferMem* buffer, unsigned int* buffer_memory);
void printNumber(int numberToPrint, int xStart, int yStart, struct bufferMem* buffer, unsigned int* buffer_memory);
unsigned int getPixelColor(int X, int Y, struct bufferMem* buffer, unsigned int* buffer_memory);
unsigned int compareColors(unsigned int colorTop, unsigned int colorBottom);
void drawImage(int x, int y, float scale, BMPIN* image, struct bufferMem* buffer, unsigned int* buffer_memory);
void getFiles(std::string path, std::string fileExtention, textLine** fileLinkedList, int startinX, int startingY);

char capitalize(char text);

#endif