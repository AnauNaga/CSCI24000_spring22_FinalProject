#ifndef FINALPROJECT_H_INCLUDED
#define FINALPROJECT_H_INCLUDED
//windows includes
#include "windows.h"
#include "framework.h"
#include <tchar.h>//used to create a LPCWSTR variable
#include <math.h>
#include "resource.h"

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
#include "DeleteFile.h"
#include "ViewFile.h"
#include "LoadFiles.h"
#include "TextField.h"
#include "SpaceObject.h"
#include "CreateNewFile.h"


struct Globals {
    //global program run status. (This is for the window, but if the window closes then the program is done)
    bool runningWindow = true;

    //global status for all buttons
    struct Input* keyInput;


    //global cursor position
    int cursorX = 0;
    int cursorY = 0;
};

LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);

int proccessMessages(Input* keyInput, HWND window, struct Globals* Global, struct bufferMem* buffer);

int mainMenu(HWND window, HDC hdc, struct Globals* Global, bufferMem* buffer);

#endif

