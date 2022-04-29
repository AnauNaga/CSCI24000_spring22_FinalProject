#include "CreateNewFile.h"

enum TextFieldList{
    FileName,
    SpaceBodyName,
    OrbitRadius,
    ConnectToFile,
    TextField_Count
};


int createNewFile(HWND window, HDC hdc, bufferMem* buffer, char* path, Globals* Global, std::string addToThisFile) {
    setWindowColor(0x00009900, buffer, (unsigned int*)buffer->background);
    setWindowColor(0xFF000000, buffer, (unsigned int*)buffer->interactable);

    TextField** TextFields = new TextField*[TextField_Count];
    TextFields[FileName] = new TextField(407, 687, 40, (unsigned int*)buffer->background, buffer->width, Global, buffer);
    TextFields[FileName]->displayTextField((unsigned int*)buffer->background);
    TextFields[SpaceBodyName] = new TextField(407, 638, 40, (unsigned int*)buffer->background, buffer->width, Global, buffer);
    TextFields[SpaceBodyName]->displayTextField((unsigned int*)buffer->background);
    TextFields[OrbitRadius] = new TextField(407, 540, 40, (unsigned int*)buffer->background, buffer->width, Global, buffer);
    TextFields[OrbitRadius]->displayTextField((unsigned int*)buffer->background);
    TextFields[ConnectToFile] = new TextField(407, 491, 40, (unsigned int*)buffer->background, buffer->width, Global, buffer);
    TextFields[ConnectToFile]->displayTextField((unsigned int*)buffer->background);


    printText((char*)"File name:", 150, 700, buffer, (unsigned int*)buffer->background);
    printText((char*)"name of Object:", 30, 651, buffer, (unsigned int*)buffer->background);
    printText((char*)"Object Type:", 102, 602, buffer, (unsigned int*)buffer->background);
    printText((char*)"Orbit Radius:", 78, 553, buffer, (unsigned int*)buffer->background);
    printText((char*)"km", 1400 , 553, buffer, (unsigned int*)buffer->background);
    printText((char*)"Parent File:", 102, 504, buffer, (unsigned int*)buffer->background);
    TextField* currField = TextFields[FileName];


    int messageReturn;
    POINT typeCursor{ 415,700 };
    POINT typeCursorPrev{ 415,700 };
    POINT cursorBlinkerPrev{ 0,0 };
    char charsToPrint[3];

    Button** buttons = new Button * [2];
    buttons[CreateBack] = new Button(1200, 200, "Back", (unsigned int*)buffer->interactable, buffer->width);
    buttons[CreateBack]->is_down = false;
    buttons[CreateEnter] = new Button(1495, 687, "Enter", (unsigned int*)buffer->interactable, buffer->width);
    buttons[CreateEnter]->is_down = false;
    Button** CosmicBodies = new Button * [CosmicBodiesButton_Count + 1];
    CosmicBodies[CosmicBodiesGalaxy] = new Button(407, 589, "Galaxy", (unsigned int*)buffer->interactable, buffer->width);
    CosmicBodies[CosmicBodiesGalaxy]->is_down = false;
    CosmicBodies[CosmicBodiesStar] = new Button(664, 589, "Star", (unsigned int*)buffer->interactable, buffer->width);
    CosmicBodies[CosmicBodiesStar]->is_down = false;
    CosmicBodies[CosmicBodiesPlanet] = new Button(873, 589, "Planet", (unsigned int*)buffer->interactable, buffer->width);
    CosmicBodies[CosmicBodiesPlanet]->is_down = false;
    CosmicBodies[CosmicBodiesMoon] = new Button(1130, 589, "Moon", (unsigned int*)buffer->interactable, buffer->width);
    CosmicBodies[CosmicBodiesMoon]->is_down = false;
    CosmicBodies[CosmicBodiesButton_Count] = CosmicBodies[CosmicBodiesGalaxy];
    textLine* imageList = new textLine{ 0,0,(char*)"Images", 0 };

    timeInfo time;
    QueryPerformanceCounter(&time.frame_begin_time);
    QueryPerformanceFrequency(&time.perf);
    //performance_frequency is the processor clock speed 
    time.performance_frequency = (float)time.perf.QuadPart;


    bool creatingNewFile = true;
    while (creatingNewFile) {

        messageReturn = proccessMessages(Global->keyInput, window, Global, buffer);
        if (!Global->runningWindow) { creatingNewFile = false; }


        //process message Return into actions
        if (Global->keyInput->buttons[BUTTON_MOUSELEFT].is_down) {
            int size = sizeof(*buttons);
            int size3 = sizeof(buttons);
            int size2 = sizeof(buttons[0]);
            for (int i = 0; i < CreateButton_Count; i++) {
                Range range = buttons[i]->getRange();
                if ((Global->cursorX > range.xMin) && (Global->cursorX < range.xMax) && (Global->cursorY > range.yMin) && (Global->cursorY < range.yMax)) {
                    buttons[i]->is_down = true;
                }
            }
            for (int i = 0; i < CosmicBodiesButton_Count; i++) {
                Range range = CosmicBodies[i]->getRange();
                if ((Global->cursorX > range.xMin) && (Global->cursorX < range.xMax) && (Global->cursorY > range.yMin) && (Global->cursorY < range.yMax)) {

                    CosmicBodies[CosmicBodiesButton_Count]->releaseButton((unsigned int*)buffer->interactable);
                    CosmicBodies[CosmicBodiesButton_Count]->is_down = false;
                    CosmicBodies[CosmicBodiesButton_Count] = CosmicBodies[i];
                    CosmicBodies[i]->pressButton((unsigned int*)buffer->interactable);
                    CosmicBodies[i]->is_down = true;
                }
            }
            for (int i = 0; i < TextField_Count; i++) {
                Range range = TextFields[i]->getRange();
                if ((Global->cursorX > range.xMin) && (Global->cursorX < range.xMax) && (Global->cursorY > range.xMin) && (Global->cursorY < range.yMax)) {
                    currField = TextFields[i];
                }
            
            }
        }
        if (Global->keyInput->buttons[BUTTON_BACK].is_down) {
            int length = strlen(currField->getText());
            if (length > 1) {

                POINT p = currField->getCoords();
                DrawRect(p.x, p.y, p.x + 24, p.y + 29, 0xFF000000, buffer, (unsigned int*)buffer->interactable);
                currField->removeLetter();

                p = currField->getCoords();
                DrawRect(p.x, p.y, p.x + 24, p.y + 29, 0xFF000000, buffer, (unsigned int*)buffer->interactable);
            }
            Global->keyInput->buttons[BUTTON_BACK].is_down = false;
        }
        if (Global->keyInput->buttons[BUTTON_CHARACTER].is_down) {
            //our text will be saved in an array of letters. when enter is pressed then we will be on a new line
            // save the old line into a linked list of all our lines and restart our array of letters

            //resize char[] to accomodate more letters each time a letter is recieved
            if (strlen(currField->getText()) < 41) {
                if (((messageReturn & 0x0000FFFF) > 47) && ((messageReturn & 0x0000FFFF) < 58) && (currField == TextFields[OrbitRadius]) || (currField != TextFields[OrbitRadius])) {
                    POINT p = currField->getCoords();

                    char incomingChar = (char)(messageReturn & 0x0000FFFF);
                    int length = strlen(currField->getText());
                    char test = currField->getText()[length - 1];
                    bool test1 = (currField->getText()[length - 2] == ' ');
                    bool test2 = (incomingChar == ' ');
                    //if both the incoming character and the previous characters are spaces then dont print
                    if ((!((incomingChar == ' ') && (currField->getText()[length - 2] == ' '))) && (!((incomingChar == ' ') && (length - 2 < 0)))) {
                        currField->Addletter(incomingChar);
                        charsToPrint[0] = incomingChar;
                        charsToPrint[1] = '\0';
                    }
                    Global->keyInput->buttons[messageReturn >> 16].is_down = false;
                    Global->keyInput->buttons[BUTTON_CHARACTER].is_down = false;
                    messageReturn = 0;
                }
            }
            else {
                DrawRect(421, 750, 1500, 779, 0x00009900, buffer, (unsigned int*)buffer->background);
                printText((char*)"File Name can only be 40 characters", 421, 750, buffer, (unsigned int*)buffer->background);
            }
        }
        //from the output of if (keyInput->buttons[BUTTON_MOUSELEFT].is_down)
        if (buttons[CreateBack]->is_down) {
            buttons[CreateBack]->pressButton((unsigned int*)buffer->interactable);
            creatingNewFile = false;
        }
        else { buttons[CreateBack]->releaseButton((unsigned int*)buffer->interactable); }
        if (buttons[CreateEnter]->is_down) {
            //only create the file if the first textfield is filled out and the second textfield is filled out
            bool continueToEnter = true;
            for (int i = 0; i < TextField_Count; i++) {
                if (strlen(TextFields[i]->getText()) <= 1) {
                    continueToEnter = false;
                }
            }
            textLine* Node = new textLine;

            while (strcmp(TextFields[ConnectToFile]->getText(), Node->text)) {
            
            }

            if (continueToEnter) {
                Range range = buttons[CreateEnter]->getRange();
                DrawRect(range.xMin, range.yMin, range.xMax, range.yMax, 0xFF000000, buffer, (unsigned int*)buffer->interactable);
                buttons[CreateEnter]->pressButton((unsigned int*)buffer->interactable);
                char* assembleFileName = TextFields[FileName]->getText();
                int wordLength = strlen(assembleFileName) - 1;
                path[0] = 'd';
                path[1] = 'a';
                path[2] = 't';
                path[3] = 'a';
                path[4] = '/';
                for (int i = 0; i < wordLength; i++) {
                    path[i + 5] = assembleFileName[i];
                }
                path[wordLength + 5] = '.';
                path[wordLength + 6] = 'd';
                path[wordLength + 7] = 'a';
                path[wordLength + 8] = 't';
                path[wordLength + 9] = '\0';
                std::ifstream check;
                check.open((const char*)path, std::ios::out | std::ios::binary);
                if (check.is_open()) {
                    DrawRect(421, 750, 1500, 779, 0x00009900, buffer, (unsigned int*)buffer->background);
                    printText((char*)"File already exists", 421, 750, buffer, (unsigned int*)buffer->background);
                }
                else {
                    std::ofstream file;
                    file.open((const char*)path, std::ios::out | std::ios::binary);
                    file << TextFields[ConnectToFile]->getText() << std::endl;
                    file.write((const char*)assembleFileName, wordLength);
                    file << "," << CosmicBodies[CosmicBodiesButton_Count]->getText() << "," << TextFields[SpaceBodyName]->getText() << "," << TextFields[OrbitRadius]->getText();
                    file.close();
                    file.open((const char*)addToThisFile.c_str(), std::fstream::in | std::fstream::out | std::fstream::app | std::ios::binary);
                    file << "," << path;
                    return 1;
                }
            }
            else {
                DrawRect(421, 750, 1500, 779, 0x00009900, buffer, (unsigned int*)buffer->background);
                printText((char*)"Please Fill out all fields", 421, 750, buffer, (unsigned int*)buffer->background);
            }
            buttons[CreateEnter]->is_down = false;
        }
        else { buttons[CreateEnter]->releaseButton((unsigned int*)buffer->interactable); }

        //set the current state of all object buttons
        for (int i = 0; i < CosmicBodiesButton_Count; i++) {
            Range range = CosmicBodies[i]->getRange();
            if (CosmicBodies[CosmicBodiesButton_Count] != CosmicBodies[i]) {
                CosmicBodies[i]->releaseButton((unsigned int*)buffer->interactable);
            }
        }
        CosmicBodies[CosmicBodiesButton_Count]->pressButton((unsigned int*)buffer->interactable);

        time.timeKeeperAlpha += time.delta_time;
        if (time.timeKeeperAlpha > 2.0f) {
            time.timeKeeperAlpha = 0;
        }
        else if (time.timeKeeperAlpha > 1.0f) {
            DrawRect(cursorBlinkerPrev.x, cursorBlinkerPrev.y, cursorBlinkerPrev.x + 24, cursorBlinkerPrev.y + 29, 0xFF000000, buffer, (unsigned int*)buffer->interactable);
            printText((char*)"_", currField->getCoords().x, currField->getCoords().y, buffer, (unsigned int*)buffer->interactable);
            cursorBlinkerPrev = { currField->getCoords().x ,currField->getCoords().y };
        }
        else {
            DrawRect(currField->getCoords().x, currField->getCoords().y, currField->getCoords().x + 24, currField->getCoords().y + 29, 0xFF000000, buffer, (unsigned int*)buffer->interactable);
        }


        //print text in text fields
        for (int i = 0; i < TextField_Count; i++) {
            printText(TextFields[i]->getText(), TextFields[i]->getTextCoords().x, TextFields[i]->getTextCoords().y, buffer, (unsigned int*)buffer->interactable);
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
