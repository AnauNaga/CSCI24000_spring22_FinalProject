#include <iostream>
#include <stdio.h>
#include "BMPIN.h"
#include "BMPOUT.h"
#include "framework.h"
#include "GuiUtilites.h"
#include <cmath>

//set the color of a pixel at coordinates (X,Y)
int setPixelColor(int X, int Y, unsigned int color, bufferMem buffer, unsigned int* buffer_memory);

//set a rectangle to a color
int DrawRect(int X1, int Y1, int X2, int Y2, unsigned int color, bufferMem buffer, unsigned int* buffer_memory);

//Draw a line from coordinates (firstX,firstY) to (secondX,secondY)
int drawLine(struct bufferMem buffer, int firstX, int firstY, int secondX, int secondY, int thickness);

//set the color of a buffer layer
int setWindowColor(unsigned int color, struct bufferMem buffer, unsigned int* buffer_memory);

//print text at coordiantes xStart, yStart
void printText(char textToPrint[], Letter* Letters[], int xStart, int yStart, struct bufferMem buffer, unsigned int* buffer_memory);
void printNumber(int numberToPrint, int xStart, int yStart, struct bufferMem buffer, unsigned int* buffer_memory);

//get the pixel color at (X,Y) - This includes oppacity information
unsigned int getPixelColor(int X, int Y, struct bufferMem buffer, unsigned int* buffer_memory);

//if the color "colorTop" is above "colorBottom" then return the color that can be seen depending on the oppacity of the two
unsigned int compareColors(unsigned int colorTop, unsigned int colorBottom);

//draw an image at coordinates (x,y) and scale out of 1000
void drawImage(int x, int y, float scale, BMPIN* image, struct bufferMem buffer, unsigned int* buffer_memory);



int setPixelColor(int X, int Y, unsigned int color, struct bufferMem buffer, unsigned int* buffer_memory) {
    unsigned int* pixel = buffer_memory;
    //if the buffer hasnt been created then dont run this function
    if (buffer.memory == 0) { return 0; }

    //if the setPixel X and Y coords are outside the buffer range then set it to the nearest pixel inside the screen
    if (X > buffer.width) { X = buffer.width - 1; }
    if (X < 0) { X = 1; }
    if (Y > buffer.height - 1) { Y = buffer.height - 1; }
    if (Y < 0) { Y = 1; }

    //if the pixel is opaque then set the pixel at (X,Y) to color
    //all pixels use 32 bits, but only 24 bits are used for color.
    // the remaining 8 bits are opacity where 00 is opaque and FF is transparent
    // E.G. 0xFF999999 is transparent and 0x00999999 is opaque and gray
    if ((color >> 24) != 0xFF) {
        pixel = pixel + X;
        pixel = pixel + (Y * buffer.width);
        *pixel = color;
    }
    return 0;
}

int DrawRect(int X1, int Y1, int X2, int Y2, unsigned int color, bufferMem buffer, unsigned int* buffer_memory){
    int ydirection = 1;
    int xdirection = 1;
    
    if ((X2 - X1) >> 31 == 1) { xdirection = -1; }
    if ((Y2 - Y1) >> 31 == 1) { ydirection = -1; }
    for (int y = Y1; y != Y2 ;y += ydirection) {
        for (int x = X1; x != X2; x += xdirection) {
            *(unsigned int*)(buffer_memory + x + y * buffer.width) = color;
        }
    }
    return 0;
}

int drawLine(struct bufferMem buffer, int firstX, int firstY, int secondX, int secondY, int thickness) {
        float slope = (float)(secondY - firstY) / (float)(secondX - firstX);

        if (slope > 1) {
            int A = 2 * (secondX - firstX);
            int B = A - (2 * (secondY - firstY));
            int P = A - (secondY - firstY);
            int range = secondY - firstY;
            int x = firstX;
            int y = firstY;
            for (int i = 0; i < range; i++) {
                if (P < 0) {
                    setPixelColor(x, y + i, 0, buffer, (unsigned int*)buffer.background);
                    P = P + A;
                }
                else if (P >= 0) {
                    x += 1;
                    setPixelColor(x, y + i, 0, buffer, (unsigned int*)buffer.background);
                    P = P + B;
                }
            }
        }
        else if (slope >= 0) {
            int A = 2 * (secondY - firstY);
            int B = A - (2 * (secondX - firstX));
            int P = A - (secondX - firstX);
            int range = secondX - firstX;
            int x = firstX;
            int y = firstY;
            for (int i = 0; i < range; i++) {
                if (P < 0) {
                    setPixelColor(x + i, y, 0, buffer, (unsigned int*)buffer.background);
                    P = P + A;
                }
                else if (P >= 0) {
                    y += 1;
                    setPixelColor(x + i, y, 0, buffer, (unsigned int*)buffer.background);
                    P = P + B;
                }
            }
        }

        return 0;
    }

int setWindowColor(unsigned int color, struct bufferMem buffer, unsigned int* buffer_memory) {
        unsigned int* pixel = buffer_memory;
        unsigned int pixelColor = 0;
        int fourthBuffer = buffer.width / 4;
        for (int y = 0; y < buffer.height; y++) {
            pixelColor = 0;
            for (int x = 0; x < buffer.width; x++) {
                *pixel = color;
                pixel++;
            }

        }

        return 0;
    }

unsigned int getPixelColor(int X, int Y, bufferMem buffer, unsigned int* buffer_memory) {
        unsigned int* pixel = (unsigned int*)buffer.memory;
        pixel = pixel + X;
        pixel = pixel + Y * buffer.width;
        return *pixel;

    }

void printText(char textToPrint[], Letter* Letters[], int xStart, int yStart, struct bufferMem buffer, unsigned int* buffer_memory) {
    //the offset of each consecutive letter so letters dont overlap
    int xOffset = xStart;
    int yOffset = yStart;
    int SizeOfTextToPrint = strlen(textToPrint);

    Letter* newLetter = new Letter[SizeOfTextToPrint];
    for (int currChar = 0; currChar < SizeOfTextToPrint; currChar++) {

        BMPIN* image = new BMPIN();
        if (textToPrint[currChar] == (char)':') {
            char fileName[] = { "Text/col.BMP" };
            image->Import(fileName);
        }else if (textToPrint[currChar] == (char)'\\'){
            char fileName[] = { "Text/bks.BMP" };
            image->Import(fileName);
        }
        else if (textToPrint[currChar] == (char)'/') {
            char fileName[] = { "Text/fws.BMP" };
            image->Import(fileName);
        }else {
            char fileName[] = { "Text/ .BMP" };
            fileName[5] = textToPrint[currChar];
            image->Import(fileName);
        }
        //all BMP files for letters will be *.BMP where * is the given file name.

        unsigned int currPixel;
        for (int y = 0; y < image->getHeight(); y++) {
            for (int x = 0; x < image->getWidth(); x++) {

                currPixel = image->getColorHex(x, y);
                if (currPixel != 0x00FFFFFF) {
                    setPixelColor(x + xOffset, y + yOffset, currPixel, buffer, (unsigned int*)buffer.background);
                }
            }//end for

        }//end for
        xOffset += 24;
        delete image;
    }//end for
}
void printNumber(int numberToPrint, int xStart, int yStart, struct bufferMem buffer, unsigned int* buffer_memory) {
    //the offset of each consecutive letter so letters dont overlap
    int xOffset = xStart;
    int yOffset = yStart;
    int digits = log10(numberToPrint+1) + 1;
    char fileName[] = { "Text/ .BMP" };
    for (int currChar = 0; currChar < digits; currChar++) {
        
        fileName[5] = static_cast<char>(numberToPrint / 100 + 48);
        numberToPrint = (numberToPrint % 100) * 10;
        //all BMP files for letters will be *.BMP where * is a single letter.

        BMPIN* image = new BMPIN();
        image->Import(fileName);
        unsigned int currPixel;
        for (int y = 0; y < image->getHeight(); y++) {
            for (int x = 0; x < image->getWidth(); x++) {

                currPixel = image->getColorHex(x, y);
                if (currPixel != 0x00FFFFFF) {
                    //setPixelColor(x + xOffset, y + yOffset, currPixel, buffer, (unsigned int*)buffer.background);
                    *(unsigned int*)((unsigned int*)buffer.background + x + xOffset + (y + yOffset) * buffer.width) = currPixel;
                }
            }//end for

        }//end for
        xOffset += 24;
        delete image;
    }//end for
}

unsigned int compareColors(unsigned int colorTop, unsigned int colorBottom){
        if ((colorTop >> 24) == 0xFF) {
            return colorBottom;
        }else {
            return colorTop;
        }

    }

void drawImage(int x, int y, float scale, BMPIN* image, struct bufferMem buffer, unsigned int* buffer_memory) {
        unsigned int currPixel;;
        float pixelSkip = 1 / scale;
        for (int imageY = 0; imageY < (float)image->getHeight() * scale; imageY++) {
            for (int imageX = 0; imageX < (float)image->getWidth() * scale; imageX++) {
                currPixel = image->getColorHex((int)(imageX * pixelSkip),(int)(imageY * pixelSkip));
                setPixelColor(imageX + x,imageY + y, currPixel, buffer, buffer_memory);
            }//end for
        }//end for

    }