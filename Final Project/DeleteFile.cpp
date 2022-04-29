#include "DeleteFile.h"


int deleteFile(HWND window, HDC hdc,bufferMem* buffer,Globals* Global) {
    setWindowColor(0x00CC3333, buffer, (unsigned int*)buffer->background);
    setWindowColor(0xFF000000, buffer, (unsigned int*)buffer->interactable);

    TextField* deleteFile = new TextField(407, 137, 40, (unsigned int*)buffer->background, buffer->width, Global, buffer);
    deleteFile->displayTextField((unsigned int*)buffer->background);
    printText((char*)"File name:", 150, 150, buffer, (unsigned int*)buffer->background);

    char* word = new char[2];
    word[0] = ' ';
    word[1] = '\0';


    textLine* fileList = new textLine{ 100,848,(char*)"File List",0 };
    word[0] = ' ';
    word[1] = '\0';
    bool printFileList = true;
    int messageReturn;
    POINT typeCursor{ 415,700 };
    POINT typeCursorPrev{ 415,700 };
    char charsToPrint[3];

    //My objects kept destructing when I had an array on the stack pointing to objects on the heap
    //Having a pointer to an array of pointers was the only way this seemed to work
    Button** delButtons = new Button * [3];

    delButtons[DeleteDelete] = new Button(1485, 137, "Delete", (unsigned int*)buffer->interactable, buffer->width);
    delButtons[DeleteBack] = new Button(1485, 88, "Back", (unsigned int*)buffer->interactable, buffer->width);
    delButtons[DeleteListFiles] = new Button(150, 88, "List Files", (unsigned int*)buffer->interactable, buffer->width);
    timeInfo time;
    QueryPerformanceCounter(&time.frame_begin_time);
    QueryPerformanceFrequency(&time.perf);
    //performance_frequency is the processor clock speed 
    time.performance_frequency = (float)time.perf.QuadPart;

    int scroll = 0.0f;

    bool deletingFile = true;
    printFileList = false;
    while (deletingFile) {

        messageReturn = proccessMessages(Global->keyInput, window, Global, buffer);
        if (!Global->runningWindow) { deletingFile = false; }
        POINT p;
        GetCursorPos(&p);
        ScreenToClient(window, &p);
        Global->cursorX = p.x;
        Global->cursorY = buffer->height - p.y;
        if (Global->keyInput->buttons[BUTTON_MOUSELEFT].is_down) {

            for (int i = 0; i < DeleteButton_Count; i++) {
                Range range = delButtons[i]->getRange();
                if ((Global->cursorX > range.xMin) && (Global->cursorX < range.xMax) && (Global->cursorY > range.yMin) && (Global->cursorY < range.yMax)) {
                    delButtons[i]->changed = delButtons[i]->is_down && true;
                    delButtons[i]->is_down = true;
                    delButtons[i]->pressButton((unsigned int*)buffer->interactable);
                }
            }
            Global->keyInput->buttons[BUTTON_MOUSELEFT].is_down = false;
        }
        else {
            for (int i = 0; i < DeleteButton_Count; i++) {
                delButtons[i]->changed = delButtons[i]->is_down && true;
                delButtons[i]->is_down = false;
            }
        }
        if (Global->keyInput->buttons[BUTTON_BACK].is_down) {
            int length = strlen(deleteFile->getText());
            if (length > 1) {

                POINT p = deleteFile->getCoords();
                DrawRect(p.x, p.y, p.x + 24, p.y + 29, 0xFF000000, buffer, (unsigned int*)buffer->interactable);
                deleteFile->removeLetter();

                p = deleteFile->getCoords();
                DrawRect(p.x, p.y, p.x + 24, p.y + 29, 0xFF000000, buffer, (unsigned int*)buffer->interactable);
            }
            Global->keyInput->buttons[BUTTON_BACK].is_down = false;
        }
        if (Global->keyInput->buttons[BUTTON_CHARACTER].is_down) {
            //our text will be saved in an array of letters. when enter is pressed then we will be on a new line
                    // save the old line into a linked list of all our lines and restart our array of letters

                    //resize char[] to accomodate more letters each time a letter is recieved
            if (strlen(deleteFile->getText()) < 41) {
                POINT p = deleteFile->getCoords();
                DrawRect(p.x, p.y, p.x + 24, p.y + 29, 0xFF000000, buffer, (unsigned int*)buffer->interactable);
                deleteFile->Addletter((char)(messageReturn & 0x0000FFFF));
                charsToPrint[0] = (char)(messageReturn & 0x0000FFFF);
                charsToPrint[1] = '\0';
                typeCursor.x += 24;
                Global->keyInput->buttons[messageReturn >> 16].is_down = false;
                Global->keyInput->buttons[BUTTON_CHARACTER].is_down = false;
                messageReturn = 0;
            }
            else {
                DrawRect(407, 190, 1500, 234, 0x00CC3333, buffer, (unsigned int*)buffer->background);
                printText((char*)"File Name can only be 40 characters", 407, 190, buffer, (unsigned int*)buffer->background);
            }
        }
        if (Global->keyInput->buttons[BUTTON_RETURN].is_down) {

            deletingFile = false;
        }
        //change scroll height;
        if (Global->keyInput->buttons[BUTTON_WHEEL].is_down) {
            int value = ((short signed int)messageReturn);
            int PrevScroll = scroll;
            scroll = scroll + (value / 10);
            if (fileList->y + scroll < 350) { scroll = PrevScroll; }
            textLine* node = fileList;
            while (node->nextNode != 0) {
                node = node->nextNode;
            }
            if (node->y + scroll > 750) { scroll = PrevScroll; }
            Global->keyInput->buttons[BUTTON_WHEEL].is_down = false;
        }


        if (delButtons[DeleteBack]->is_down) {
            delButtons[DeleteBack]->pressButton((unsigned int*)buffer->interactable);
            deletingFile = false;
        }
        else { delButtons[DeleteBack]->releaseButton((unsigned int*)buffer->interactable); }
        if (delButtons[DeleteDelete]->is_down) {
            std::string path = "./Data";
            std::string directory;
            std::string currFile;
            std::stringstream assembleFile;
            std::stringstream sstream;
            std::stringstream converter;
            std::string Data;

            char* fileToDelete = { deleteFile->getText() };
            if (fileToDelete[strlen(fileToDelete) - 1] == ' ') {
                fileToDelete[strlen(fileToDelete) - 1] = '\0';
            }


            assembleFile << path << "/";
            textLine** fileListPtr = &fileList;
            getFiles(path, ".dat", fileListPtr, 100, 800);
            textLine* Node = fileList;
            textLine* PrevNode = 0;
            textLine* fileFound = Node;
            char* capitalFile = {(char*)" "};
            int length;
            while (strcmp(capitalFile, fileToDelete) && Node->nextNode) {

                fileFound = Node->nextNode;
                capitalFile = new char[strlen(fileFound->text) + 2];
                PrevNode = Node;
                Node = Node->nextNode;
                length = strlen(fileFound->text);
                for (int i = 0; i < strlen(fileFound->text); i++) {
                    capitalFile[i] = static_cast<char>(std::toupper(fileFound->text[i])); //this is wierd how toUpper handles characters as integers
                }
                capitalFile[strlen(fileFound->text)] = 0;

            }
            if (strcmp(capitalFile, fileToDelete)) {
                //If the file wasn't found then print "file does not exist"
                DrawRect(407, 190, 1500, 234, 0x00007700, buffer, (unsigned int*)buffer->background);
                DrawRect(407, 190, 1500, 238, 0xFF000000, buffer, (unsigned int*)buffer->background);
                deleteFile->reset();
                printText((char*)"File does not exist", 407, 190, buffer, (unsigned int*)buffer->background);
            }
            else {
                assembleFile << Node->text;
                assembleFile << ".dat";
                getline(assembleFile, currFile);
                if (Node->nextNode != 0) {
                    PrevNode->nextNode = Node->nextNode;
                }
                Node->nextNode = 0;
                delete Node;
                remove(currFile.c_str());
                deleteFile->reset();
            }

            delButtons[DeleteListFiles]->is_down = true;
        }
        else { delButtons[DeleteDelete]->releaseButton((unsigned int*)buffer->interactable); }
        Range range = delButtons[DeleteListFiles]->getRange();
        if (delButtons[DeleteListFiles]->is_down) {
            printFileList = true;
            std::string path = "./Data";
            std::string directory;
            std::stringstream sstream;
            std::stringstream converter;
            std::string Data;
            int line = 800;
            textLine** Node = &fileList;
            getFiles(path, ".dat", Node, 100, 700);
            DrawRect(100, 300, 900, 800, 0x00AA3333, buffer, (unsigned int*)buffer->background);
            delButtons[DeleteListFiles]->pressButton((unsigned int*)buffer->interactable);
            delButtons[DeleteListFiles]->is_down = false;
        }
        else { delButtons[DeleteListFiles]->releaseButton((unsigned int*)buffer->interactable); }


        time.timeKeeperAlpha += time.delta_time;
        if (time.timeKeeperAlpha > 2.0f) {
            time.timeKeeperAlpha = 0;
        }
        else if (time.timeKeeperAlpha > 1.0f) {
            printText((char*)"_", deleteFile->getCoords().x, deleteFile->getCoords().y, buffer, (unsigned int*)buffer->interactable);
        }
        else {
            DrawRect(deleteFile->getCoords().x, deleteFile->getCoords().y, deleteFile->getCoords().x + 24, deleteFile->getCoords().y + 29, 0xFF000000, buffer, (unsigned int*)buffer->interactable);
        }


        printText(deleteFile->getText(), deleteFile->getTextCoords().x, deleteFile->getTextCoords().y, buffer, (unsigned int*)buffer->interactable);

        //for our file list, draw every file
        textLine* nextNode = fileList;
        printText(word, typeCursorPrev.x, typeCursorPrev.y, buffer, (unsigned int*)buffer->interactable);
        int yScrollMax = 800 - scroll;
        int yScrollMin = 260 - scroll;
        bool procceedingFilesAreVisible = true;
        if (printFileList) {
            while (nextNode && procceedingFilesAreVisible) {
                if (nextNode->y < yScrollMin) { procceedingFilesAreVisible = false; }
                if ((nextNode->y > yScrollMin) && (nextNode->y < yScrollMax) && (procceedingFilesAreVisible)) {
                    printText(nextNode->text, nextNode->x, nextNode->y + scroll, buffer, (unsigned int*)buffer->interactable);
                }
                nextNode = nextNode->nextNode;

            }
            DrawRect(100, 250, 900, 300, 0x00CC3333, buffer, (unsigned int*)buffer->interactable);
            DrawRect(100, 800, 900, 850, 0x00CC3333, buffer, (unsigned int*)buffer->interactable);
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