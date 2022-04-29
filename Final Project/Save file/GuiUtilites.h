#ifndef GUIUTILITES_H_INCLUDED
#define GUIUTILITES_H_INCLUDED
#include "framework.h"
#include "BMPIN.h"
#include "BMPOUT.h"
struct bufferMem {
    bufferMem() {
        bufferMem::memory = 0;
        bufferMem::background = 0;
        bufferMem::interactable = 0;
        bufferMem::width = 0;
        bufferMem::height = 0;
    }
    void* memory;
    void* background;
    void* interactable;
    int width;
    int height;
    BITMAPINFO buffer_bitmap_info;
};

struct timeInfo {
    int frame_rate;
    float seconds_counter = 0.0f;
    float delta_time = 0.016666f;
    LARGE_INTEGER frame_begin_time;
    LARGE_INTEGER frame_end_time;

    //performance_frequency is the processor clock speed 
    float performance_frequency;
    LARGE_INTEGER perf;

};

struct Letter {
    int x;
    int y;
    char Letter;
};
int setPixelColor(int X, int Y, unsigned int color, bufferMem buffer, unsigned int* buffer_memory);
int DrawRect(int X1, int Y1, int X2, int Y2, unsigned int color, bufferMem buffer, unsigned int* buffer_memory);
int drawLine(struct bufferMem buffer, int firstX, int firstY, int secondX, int secondY, int thickness);
int setWindowColor(unsigned int color, struct bufferMem buffer, unsigned int* buffer_memory);
void printText(char textToPrint[], Letter* Letters[], int xStart, int yStart, struct bufferMem buffer, unsigned int* buffer_memory);
void printNumber(int numberToPrint, int xStart, int yStart, struct bufferMem buffer, unsigned int* buffer_memory);
unsigned int getPixelColor(int X, int Y, struct bufferMem buffer, unsigned int* buffer_memory);
unsigned int compareColors(unsigned int colorTop, unsigned int colorBottom);
void drawImage(int x, int y, float scale, BMPIN* image, struct bufferMem buffer, unsigned int* buffer_memory);

#endif