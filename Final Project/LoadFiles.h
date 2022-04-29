#ifndef LOADFILES_H_INCLUDED
#define LOADFILES_H_INCLUDED
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
#include "TextField.h"
#include "SpaceObject.h"
#include "CreateNewFile.h"


enum Load {
    LoadLoad,
    LoadBack,
    LoadListFiles,
    LoadButton_Count


};

int loadFile(HWND window, HDC hdc,struct bufferMem* buffer, struct Globals* Global);

#endif 

