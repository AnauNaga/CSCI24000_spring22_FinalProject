#ifndef DELETEFILE_H_INCLUDED
#define DELETEFILE_H_INCLUDED

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
#include "SpaceObject.h"
#include "CreateNewFile.h"

enum Delete {
    DeleteDelete,
    DeleteBack,
    DeleteListFiles,
    DeleteButton_Count
};

int deleteFile(HWND window, HDC hdc, struct bufferMem* buffer, struct Globals* Global);

#endif
