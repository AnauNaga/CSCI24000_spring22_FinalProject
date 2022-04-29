#include "ViewFile.h"
//windows includes
#include "windows.h"
#include "framework.h"
#include <tchar.h>//used to create a LPCWSTR variable
#include <math.h>

//tools
#include <string>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <fileSystem>
#include <fstream>
#include <sstream>

//local includes
#include "Button.h"
#include "KeyTracker.h"
#include "BMPIO.h"
#include "BMPOUT.h"
#include "BMPIN.h"
#include "GuiUtilites.h"
#include "TextField.h"
#include "Final Project.h"




enum View {
    ViewBack,
    ViewAdd,
    ViewButtion_Count
};
int viewFile(HWND window, HDC hdc,bufferMem* buffer, char* filePath, Globals* Global) {


    //preload mars and Planet images
    BMPIN* Mars = new BMPIN();
    Mars->Import("text/Mars.bmp", 0x00000000);
    BMPIN* Planet = new BMPIN();
    Planet->Import("text/Planet.bmp", 0x00000000);
    BMPIN* Sun = new BMPIN();
    Sun->Import("text/Sun.bmp", 0x00000000);
    int SunX = 950;
    int SunY = 450;
    double PlanetTheta = 3.14;
    double moonTheta = 0;


    setWindowColor(0xFF000000, buffer, (unsigned int*)buffer->background);
    setWindowColor(0xFF000000, buffer, (unsigned int*)buffer->interactable);


    BMPIN* image = new BMPIN;
    image->Import("text/SystemFull.bmp", 0xFF000000);
    drawImage(0, 0, 1, image, buffer, (unsigned int*)buffer->background);
    DrawRect(0, 0, buffer->width, 150, 0x00444444,buffer, (unsigned int*)buffer->background);
    std::ifstream file;


    file.open((const char*)filePath, std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        printText((char*)"File Failed To open", 400, 500, buffer, (unsigned int*)buffer->background);
        printText((char*)"Click anywhere to exit", 352, 452, buffer, (unsigned int*)buffer->background);
        bool waitingToReturn = true;
        int messageReturn;
        Global->keyInput->buttons[BUTTON_MOUSELEFT].is_down = false;
        while (waitingToReturn) {
            messageReturn = proccessMessages(Global->keyInput, window, Global, buffer);
            if (!Global->runningWindow) { waitingToReturn = false; }
            if (Global->keyInput->buttons[BUTTON_MOUSELEFT].is_down) {
                return 0;
            }
        }
    }
    Global->keyInput->buttons[BUTTON_MOUSELEFT].is_down = false;

    int messageReturn;
    //My objects kept destructing when I had an array on the stack pointing to objects on the heap
    //Having a pointer to an array of pointers was the only way this seemed to work
    Button** viewButtons = new Button * [ViewButtion_Count];
    viewButtons[ViewBack] = new Button(1485, 88, "Back", (unsigned int*)buffer->interactable, buffer->width);
    viewButtons[ViewAdd] = new Button(1485, 40, "Add an object", (unsigned int*)buffer->interactable, buffer->width);


    timeInfo time;
    QueryPerformanceCounter(&time.frame_begin_time);
    QueryPerformanceFrequency(&time.perf);
    //performance_frequency is the processor clock speed 
    time.performance_frequency = (float)time.perf.QuadPart;

    int scroll = 0.0f;
    bool viewingBody = true;
    while (viewingBody) {
        messageReturn = proccessMessages(Global->keyInput, window, Global, buffer);

        if (Global->keyInput->buttons[BUTTON_MOUSELEFT].is_down) {
            for (int i = 0; i < ViewButtion_Count; i++) {
                Range range = viewButtons[i]->getRange();
                if ((Global->cursorX > range.xMin) && (Global->cursorX < range.xMax) && (Global->cursorY > range.yMin) && (Global->cursorY < range.yMax)) {
                    viewButtons[i]->is_down = true;
                }
            }

        }
        if (viewButtons[ViewBack]->is_down) {
            Global->keyInput->buttons[BUTTON_MOUSELEFT].is_down = false;
            viewButtons[ViewBack]->pressButton((unsigned int*)buffer->interactable);
            viewingBody = false;
        }
        else { viewButtons[ViewBack]->releaseButton((unsigned int*)buffer->interactable); }
        if (viewButtons[ViewAdd]->is_down) {
            char* path = new char[50];
            createNewFile(window, hdc, buffer, path, Global, filePath);

            setWindowColor(0xFF000000, buffer, (unsigned int*)buffer->background);
            setWindowColor(0xFF000000, buffer, (unsigned int*)buffer->interactable);
            drawImage(0, 0, 1, image, buffer, (unsigned int*)buffer->background);
            DrawRect(0, 0, buffer->width, 150, 0x00444444, buffer, (unsigned int*)buffer->background);

            viewButtons[ViewAdd]->is_down = false;
        }
        else {
            viewButtons[ViewAdd]->releaseButton((unsigned int*)buffer->interactable);
        }


        PlanetTheta = PlanetTheta + (0.1f * time.delta_time);
        moonTheta = moonTheta + (1.5f * time.delta_time);
        if (PlanetTheta > 6.283184) { PlanetTheta = 0; }
        if (moonTheta > 6.283184) { moonTheta = 0; }


        int PlanetX = SunX + 20 * cos(PlanetTheta + 3.14f);
        int PlanetY = SunY + 2 * sin(PlanetTheta + 3.14f);
        int Planetz = PlanetY;
        float Sunscale = 250 / (float)(PlanetY);
        int Plax = SunX + 400 * cos(PlanetTheta);
        int Play = SunY + 40 * sin(PlanetTheta);
        int Plaz = Play;
        float plascale = 50 / (float)(Play - 350);
        int moox = Plax + 400 * plascale * cos(moonTheta);
        int mooy = Play + 40 * plascale * sin(moonTheta);
        int mooz = mooy;
        float mooscale = 30 / (float)(mooy - 350);
        PlanetX -= Sunscale * 100 / 2;
        PlanetY -= Sunscale * 100 / 2;
        Plax -= plascale * 100 / 2;
        Play -= plascale * 100 / 2;
        moox -= mooscale * 100 / 2;
        mooy -= mooscale * 100 / 2;
        if (Plaz > Planetz) {
            if (mooz > Plaz) {
                drawImage(moox, mooy, mooscale, Mars, buffer, (unsigned int*)buffer->interactable);
                drawImage(Plax, Play, plascale, Planet, buffer, (unsigned int*)buffer->interactable);
            }
            else {
                drawImage(Plax, Play, plascale, Planet, buffer, (unsigned int*)buffer->interactable);
                drawImage(moox, mooy, mooscale, Mars, buffer, (unsigned int*)buffer->interactable);
            }
            drawImage(PlanetX, PlanetY, Sunscale, Sun, buffer, (unsigned int*)buffer->interactable);
        }
        else {

            drawImage(PlanetX, PlanetY, Sunscale, Sun, buffer, (unsigned int*)buffer->interactable);
            if (mooz > Plaz) {
                drawImage(moox, mooy, mooscale, Mars, buffer, (unsigned int*)buffer->interactable);
                drawImage(Plax, Play, plascale, Planet, buffer, (unsigned int*)buffer->interactable);
            }
            else {
                drawImage(Plax, Play, plascale, Planet, buffer, (unsigned int*)buffer->interactable);
                drawImage(moox, mooy, mooscale, Mars, buffer, (unsigned int*)buffer->interactable);
            }
        }

        //combine all the bitmap layers
        unsigned int* background = (unsigned int*)buffer->background;
        unsigned int* memory = (unsigned int*)buffer->memory;
        unsigned int* interactable = (unsigned int*)buffer->interactable;
        for (int x = 0; x < buffer->width; x++) {
            for (int y = 0; y < buffer->height; y++) {
                if (*background >> 24 != 0xFF) {
                    *memory = *background;
                }
                if (*interactable >> 24 != 0xFF) {
                    *memory = *interactable;
                }
                memory++;
                background++;
                interactable++;
            }
        }

        StretchDIBits(hdc, 0, 0, buffer->width, buffer->height, 0, 0, buffer->width, buffer->height, buffer->memory, &buffer->buffer_bitmap_info, DIB_RGB_COLORS, SRCCOPY);
        setWindowColor(0xFF000000, buffer, (unsigned int*)buffer->interactable);
        //get the time a frame took to render.
        QueryPerformanceCounter(&time.frame_end_time);
        time.delta_time = (float)(time.frame_end_time.QuadPart - time.frame_begin_time.QuadPart) / time.performance_frequency;
        time.frame_begin_time.QuadPart = time.frame_end_time.QuadPart;

    }
    return 0;
}
