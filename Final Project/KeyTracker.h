#ifndef KEYTRACKER_H_INCLUDED
#define KEYTRACKER_H_INCLUDED

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


struct Button_State {
	bool is_downPrev = false;
	bool is_down = false;
	bool changed = false;
};

enum {
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_MOUSELEFT,
	BUTTON_A,
	BUTTON_B,
	BUTTON_C,
	BUTTON_D,
	BUTTON_E,
	BUTTON_F,
	BUTTON_G,
	BUTTON_H,
	BUTTON_I,
	BUTTON_J,
	BUTTON_K,
	BUTTON_L,
	BUTTON_M,
	BUTTON_N,
	BUTTON_O,
	BUTTON_P,
	BUTTON_Q,
	BUTTON_R,
	BUTTON_S,
	BUTTON_T,
	BUTTON_U,
	BUTTON_V,
	BUTTON_W,
	BUTTON_X,
	BUTTON_Y,
	BUTTON_Z,
	BUTTON_0,
	BUTTON_1,
	BUTTON_2,
	BUTTON_3,
	BUTTON_4,
	BUTTON_5,
	BUTTON_6,
	BUTTON_7,
	BUTTON_8,
	BUTTON_9,
	BUTTON_SPACE,
	BUTTON_RETURN,
	BUTTON_BACK,
	BUTTON_WHEEL,
	BUTTON_SHIFT,
	BUTTON_SUBTRACT,
	BUTTON_UNDERSCORE,
	BUTTON_PERIOD,
	BUTTON_CHARACTER,
	BUTTON_COUNT
};

struct Input {
	Button_State buttons[BUTTON_COUNT];

};

#endif

