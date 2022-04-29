#include "LoadFiles.h"

int loadFile(HWND window, HDC hdc, bufferMem* buffer, Globals* Global) {
    setWindowColor(0x00009900, buffer, (unsigned int*)buffer->background);
    setWindowColor(0xFF000000, buffer, (unsigned int*)buffer->interactable);
    printText((char*)"File name:", 150, 150, buffer, (unsigned int*)buffer->background);
    TextField* fileToLoad = new TextField(407, 137, 40, (unsigned int*)buffer->background, buffer->width, Global, buffer);
    fileToLoad->displayTextField((unsigned int*)buffer->background);

    char* word = new char[2];
    word[0] = ' ';
    word[1] = '\0';


    textLine* loadFileList = new textLine{ 100,848,(char*)"File List",0 };
    word[0] = ' ';
    word[1] = '\0';
    bool printloadFileList = true;
    int messageReturn;
    POINT typeCursor{ 415,700 };
    POINT typeCursorPrev{ 415,700 };
    char charsToPrint[3];

    //My objects kept destructing when I had an array on the stack pointing to objects on the heap
    //Having a pointer to an array of pointers was the only way this seemed to work
    Button** loadButtons = new Button * [3];
    loadButtons[LoadBack] = new Button(1485, 88, "Back", (unsigned int*)buffer->interactable, buffer->width);
    loadButtons[LoadLoad] = new Button(1485, 137, "Load", (unsigned int*)buffer->interactable, buffer->width);
    loadButtons[LoadListFiles] = new Button(150, 88, "List Files", (unsigned int*)buffer->interactable, buffer->width);
    timeInfo time;
    QueryPerformanceCounter(&time.frame_begin_time);
    QueryPerformanceFrequency(&time.perf);
    //performance_frequency is the processor clock speed 
    time.performance_frequency = (float)time.perf.QuadPart;

    int scroll = 0.0f;
    Global->keyInput->buttons[BUTTON_MOUSELEFT].is_down = false;
    bool loadingFile = true;
    printloadFileList = false;
    while (loadingFile) {

        messageReturn = proccessMessages(Global->keyInput, window, Global, buffer);
        if (!Global->runningWindow) { loadingFile = false; }

        if (Global->keyInput->buttons[BUTTON_MOUSELEFT].is_down) {

            for (int i = 0; i < LoadButton_Count; i++) {
                Range range = loadButtons[i]->getRange();
                if ((Global->cursorX > range.xMin) && (Global->cursorX < range.xMax) && (Global->cursorY > range.yMin) && (Global->cursorY < range.yMax)) {
                    loadButtons[i]->is_down = true;
                }
            }


        }
        if (Global->keyInput->buttons[BUTTON_BACK].is_down) {
            int length = strlen(fileToLoad->getText());
            if (length > 1) {

                POINT p = fileToLoad->getCoords();
                DrawRect(p.x, p.y, p.x + 24, p.y + 29, 0xFF000000, buffer, (unsigned int*)buffer->interactable);
                fileToLoad->removeLetter();

                p = fileToLoad->getCoords();
                DrawRect(p.x, p.y, p.x + 24, p.y + 29, 0xFF000000, buffer, (unsigned int*)buffer->interactable);
            }
            Global->keyInput->buttons[BUTTON_BACK].is_down = false;
        }
        if (Global->keyInput->buttons[BUTTON_CHARACTER].is_down) {
            //our text will be saved in an array of letters. when enter is pressed then we will be on a new line
                    // save the old line into a linked list of all our lines and restart our array of letters

                    //resize char[] to accomodate more letters each time a letter is recieved
            if (strlen(fileToLoad->getText()) < 41) {
                POINT p = fileToLoad->getCoords();
                DrawRect(p.x, p.y, p.x + 24, p.y + 29, 0xFF000000, buffer, (unsigned int*)buffer->interactable);
                fileToLoad->Addletter((char)(messageReturn & 0x0000FFFF));
                charsToPrint[0] = (char)(messageReturn & 0x0000FFFF);
                charsToPrint[1] = '\0';
                typeCursor.x += 24;
                Global->keyInput->buttons[messageReturn >> 16].is_down = false;
                Global->keyInput->buttons[BUTTON_CHARACTER].is_down = false;
                messageReturn = 0;
            }
            else {
                DrawRect(421, 750, 1500, 779, 0xFF000000, buffer, (unsigned int*)buffer->interactable);
                printText((char*)"File Name can only be 40 characters", 421, 750, buffer, (unsigned int*)buffer->interactable);
            }
        }
        if (Global->keyInput->buttons[BUTTON_RETURN].is_down) {

        }
        //change scroll height;
        if (Global->keyInput->buttons[BUTTON_WHEEL].is_down) {
            int value = ((short signed int)messageReturn);
            int PrevScroll = scroll;
            scroll = scroll + (value / 10);
            if (loadFileList->y + scroll < 350) { scroll = PrevScroll; }
            textLine* node = loadFileList;
            while (node->nextNode != 0) {
                node = node->nextNode;
            }
            if (node->y + scroll > 750) { scroll = PrevScroll; }
            Global->keyInput->buttons[BUTTON_WHEEL].is_down = false;
        }


        if (loadButtons[LoadBack]->is_down) {
            loadButtons[LoadBack]->pressButton((unsigned int*)buffer->interactable);
            loadingFile = false;
        }
        else { loadButtons[LoadBack]->releaseButton((unsigned int*)buffer->interactable); }
        if (loadButtons[LoadLoad]->is_down) {
            loadButtons[LoadLoad]->is_down = false;
            std::string path = "./Data";
            std::stringstream assembleFile;
            std::string currFile;
            int line = 800;

            getFiles(path, ".dat", &loadFileList, 100, 800);
            textLine* Node = loadFileList;
            textLine* fileFound = Node;
            int length = strlen(fileFound->text);
            char* capitalFile = new char[length + 1];
            capitalFile[strlen(fileFound->text)] = 0;
            fileToLoad->removeEndingSpace();
            while (strcmp(capitalFile, fileToLoad->getText()) && Node) {
                delete[] capitalFile;
                fileFound = Node;
                capitalFile = new char[strlen(fileFound->text) + 2];
                Node = Node->nextNode;
                length = strlen(fileFound->text);
                for (int i = 0; i < strlen(fileFound->text); i++) {
                    capitalFile[i] = static_cast<char>(std::toupper(fileFound->text[i])); //this is wierd how toUpper handles characters as integers
                }
                capitalFile[strlen(fileFound->text)] = 0;
            }
            int test = strcmp(fileFound->text, fileToLoad->getText());
            if (!strcmp(fileFound->text, fileToLoad->getText())) {
                assembleFile << "Data/" << fileFound->text << ".dat";
                getline(assembleFile, currFile);
                viewFile(window, hdc,buffer, (char*)currFile.c_str(), Global);
                setWindowColor(0x00009900, buffer, (unsigned int*)buffer->background);
                setWindowColor(0xFF000000, buffer, (unsigned int*)buffer->interactable);
                DrawRect(100, 300, 900, 800, 0x007700, buffer, (unsigned int*)buffer->background);
                printText((char*)"File name:", 150, 150, buffer, (unsigned int*)buffer->background);
                fileToLoad->displayTextField((unsigned int*)buffer->background);
                fileToLoad->reset();
            }
            else {
                //If the file wasn't found then print "file does not exist"
                DrawRect(407, 190, 1500, 234, 0x00007700, buffer, (unsigned int*)buffer->background);
                DrawRect(407, 190, 1500, 238, 0xFF000000, buffer, (unsigned int*)buffer->background);
                fileToLoad->reset();
                printText((char*)"File does not exist", 407, 190, buffer, (unsigned int*)buffer->background);

            }

        }//end if
        else { loadButtons[LoadLoad]->releaseButton((unsigned int*)buffer->interactable); }
        Range range = loadButtons[LoadListFiles]->getRange();
        if (loadButtons[LoadListFiles]->is_down) {
            printloadFileList = true;
            std::string path = "./Data";
            std::string directory;
            std::stringstream sstream;
            std::stringstream converter;
            std::string Data;
            int line = 800;
            textLine** Node = &loadFileList;
            getFiles(path, ".dat", Node, 100, 700);
            DrawRect(100, 300, 900, 800, 0x007700, buffer, (unsigned int*)buffer->background);
            loadButtons[LoadListFiles]->pressButton((unsigned int*)buffer->interactable);
            loadButtons[LoadListFiles]->is_down = false;
        }
        else { loadButtons[LoadListFiles]->releaseButton((unsigned int*)buffer->interactable); }


        time.timeKeeperAlpha += time.delta_time;
        if (time.timeKeeperAlpha > 2.0f) {
            time.timeKeeperAlpha = 0;
        }
        else if (time.timeKeeperAlpha > 1.0f) {
            printText((char*)"_", fileToLoad->getCoords().x, fileToLoad->getCoords().y, buffer, (unsigned int*)buffer->interactable);
        }
        else {
            DrawRect(fileToLoad->getCoords().x, fileToLoad->getCoords().y, fileToLoad->getCoords().x + 24, fileToLoad->getCoords().y + 29, 0xFF000000, buffer, (unsigned int*)buffer->interactable);
        }


        printText(fileToLoad->getText(), fileToLoad->getTextCoords().x, fileToLoad->getTextCoords().y, buffer, (unsigned int*)buffer->interactable);


        //for our file list, draw every file
        textLine* nextNode = loadFileList;
        printText(word, typeCursorPrev.x, typeCursorPrev.y, buffer, (unsigned int*)buffer->interactable);
        int yScrollMax = 800 - scroll;
        int yScrollMin = 260 - scroll;
        bool procceedingFilesAreVisible = true;
        if (printloadFileList) {
            while (nextNode && procceedingFilesAreVisible) {
                if (nextNode->y < yScrollMin) { procceedingFilesAreVisible = false; }
                if ((nextNode->y > yScrollMin) && (nextNode->y < yScrollMax) && (procceedingFilesAreVisible)) {
                    printText(nextNode->text, nextNode->x, nextNode->y + scroll, buffer, (unsigned int*)buffer->interactable);
                }
                nextNode = nextNode->nextNode;

            }
            DrawRect(100, 250, 900, 300, 0x00009900, buffer, (unsigned int*)buffer->interactable);
            DrawRect(100, 800, 900, 850, 0x00009900, buffer, (unsigned int*)buffer->interactable);
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