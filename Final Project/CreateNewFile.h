#ifndef CREATENEWFILE_H_INCLUDED
#define CREATENEWFILE_H_INCLUDED
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
#include "FinalProject.h"
#include "DeleteFile.h"
#include "ViewFile.h"
#include "LoadFiles.h"
#include "TextField.h"
#include "SpaceObject.h"
#include "CreateNewFile.h"

enum Create {
    CreateEnter,
    CreateBack,
    CreateButton_Count

};
enum CosmicBodies {
    CosmicBodiesGalaxy,
    CosmicBodiesStar,
    CosmicBodiesPlanet,
    CosmicBodiesMoon,
    CosmicBodiesButton_Count
};

int createNewFile(HWND window, HDC hdc, bufferMem* buffer, char* path, Globals* Global,std::string addToThisFile);

#endif
