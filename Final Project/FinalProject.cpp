
#include "FinalProject.h"


enum ButtonList {
    NewFile,
    LoadFile,
    ListFile,
    DelFile,
    Exit,
    ButtonCount
};

bufferMem* buffer = new bufferMem;
Globals* Global = new Globals;

//Read messages sent by windows and do something based on them.
LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;
    //Windows likes to store information in each individual bit of a byte. Get the bit reguarding if a key is pressed down from lparam.
    

    //do something based off the message read.
    switch (uMsg) {
    case WM_CLOSE:
    case WM_DESTROY: {
        Global->runningWindow = false;
    } break;
    case WM_SIZE: {

        //if the window memory == 0 then it hasn't been initialized; Initialize window memory.
        //This if statement is necessary to prevent memory buffers from being deleted upon minimizing the window.
        if (buffer->memory == 0) {
            //get window dimensins
            RECT rect;
            GetClientRect(hwnd, &rect);
            buffer->width = rect.right - rect.left;
            buffer->height = rect.bottom - rect.top;


            int buffer_size = buffer->width * buffer->height * sizeof(unsigned int);

            if (buffer->memory) VirtualFree(buffer->memory, 0, MEM_RELEASE);
            buffer->memory = VirtualAlloc(0, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
            buffer->buffer_bitmap_info.bmiHeader.biSize = sizeof(buffer->buffer_bitmap_info.bmiHeader);
            buffer->buffer_bitmap_info.bmiHeader.biWidth = buffer->width;
            buffer->buffer_bitmap_info.bmiHeader.biHeight = buffer->height;
            buffer->buffer_bitmap_info.bmiHeader.biPlanes = 1;
            buffer->buffer_bitmap_info.bmiHeader.biBitCount = 32;
            buffer->buffer_bitmap_info.bmiHeader.biCompression = BI_RGB;

            //allocate memory for buffer_background
            if (buffer->background) VirtualFree(buffer->background, 0, MEM_RELEASE);
            buffer->background = VirtualAlloc(0, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

            //allocate memory for buffer_interactable
            if (buffer->interactable) VirtualFree(buffer->interactable, 0, MEM_RELEASE);
            buffer->interactable = VirtualAlloc(0, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);


            setWindowColor(0x00000AA, buffer, (unsigned int*)buffer->background);
            setWindowColor(0xFF000000, buffer, (unsigned int*)buffer->interactable);
            printText((char*)"Cosmic Database", 200, 960, buffer, (unsigned int*)buffer->background);
            BMPIN* image = new BMPIN();
            image->Import("Text/System.bmp");
            drawImage(920, 0, 1, image, buffer, (unsigned int*)buffer->background);
            delete image;
        }
    }
    case WM_LBUTTONUP: {
        Global->keyInput->buttons[BUTTON_MOUSELEFT].is_down = false;
        Global->keyInput->buttons[BUTTON_MOUSELEFT].changed = false;
        return result;
    }break;
    default: {
        result = DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    }
    
    return result;
}
//run the window
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {


    Global->keyInput = new Input;

    //create a window and set its starting settings
    WNDCLASS window_class = {};
    window_class.style = CS_HREDRAW | CS_VREDRAW;
    window_class.lpszClassName = L"Final Project";
    window_class.lpfnWndProc = window_callback;
    RegisterClass(&window_class);
    HWND window = CreateWindow(window_class.lpszClassName, L"Cosmic Database", WS_OVERLAPPED | WS_VISIBLE | WS_MINIMIZEBOX | WS_SYSMENU, 0, 0, 1936, 1040, 0, 0, hInstance, 0);
    HDC hdc = GetDC(window);
    

    mainMenu(window, hdc, Global, buffer);
}//end winMain

int mainMenu(HWND window, HDC hdc, Globals* Global, struct bufferMem* buffer) {


    //tools used to keep track of time
    timeInfo time;
    QueryPerformanceCounter(&time.frame_begin_time);
    QueryPerformanceFrequency(&time.perf);
    //performance_frequency is the processor clock speed 
    time.performance_frequency = (float)time.perf.QuadPart;


    //current position of typing
    POINT typeCursor = { 0,0 };
    POINT typeCursorPrev = { 0,0 };
    char charsToPrint[2];


    //preload mars and Planet images
    BMPIN* Mars = new BMPIN();
    Mars->Import("text/Mars.bmp", 0x00000000);
    BMPIN* Planet = new BMPIN();
    Planet->Import("text/Planet.bmp", 0x00000000);
    BMPIN* Sun = new BMPIN();
    Sun->Import("text/Sun.bmp", 0x00000000);
    int SunX = 1475;
    int SunY = 450;
    double PlanetTheta = 3.14;
    double moonTheta = 0;
    int messageReturn;

    //linked list of words
    textLine* text = new textLine{ 0,0,0,0 };
    char* word = new char[2];
    word[0] = ' ';
    word[1] = '\0';

    textLine* fileList = new textLine{ 100,848,(char*)"File List",0 };
    word[0] = ' ';
    word[1] = '\0';
    bool printFileList = true;
    while (buffer->memory == 0) {
        messageReturn = proccessMessages(Global->keyInput, window, Global, buffer);
    }
    Button** button = new Button*[ButtonCount];
    button[NewFile] = new Button(100, 100, (std::string)"Create New File", (unsigned int*)buffer->interactable, buffer->width);
    button[LoadFile] = new Button(100, 149, (std::string)"Load File", (unsigned int*)buffer->interactable, buffer->width);
    button[ListFile] = new Button(100, 198, (std::string)"List Files", (unsigned int*)buffer->interactable, buffer->width);
    button[DelFile] = new Button(100, 51, (std::string)"Delete file", (unsigned int*)buffer->interactable, buffer->width);
    button[Exit] = new Button(1700, 51, "Exit", (unsigned int*)buffer->interactable, buffer->width);

    int scroll = 0.0f;
    //run the window
    while (Global->runningWindow) {


        messageReturn = proccessMessages(Global->keyInput, window, Global, buffer);


        //get clicky input
        if (Global->keyInput->buttons[BUTTON_MOUSELEFT].is_down) {

            if (strlen(word) > 1) {
                //create a new textLine which will save the new line of text that we made
                textLine* nextNode = text;
                while (nextNode->nextNode != 0) {
                    nextNode = nextNode->nextNode;
                }
                nextNode->nextNode = new textLine;
                nextNode->x = typeCursorPrev.x;
                nextNode->y = typeCursorPrev.y;
                nextNode->text = word;//Here we are handing our pointer to our linked list and then giving our char* word a new pointer to a new array
                word = new char[2];
                word[1] = '\0';
                word[0] = ' ';
            }
            typeCursor = { 24 * (Global->cursorX / 24),29 * (Global->cursorY / 29) };

            typeCursorPrev = typeCursor;
            Range range;
            for (int i = 0; i < ButtonCount; i++) {
                range = button[i]->getRange();
                if ((Global->cursorX < range.xMax) && (Global->cursorX > range.xMin) && (Global->cursorY < range.yMax) && (Global->cursorY > range.yMin)) {
                    button[i]->changed = button[i]->is_down != true;
                    button[i]->is_down = true;
                }
                else {
                    button[i]->changed = button[0]->is_down != false;
                    button[i]->is_down = false;
                    button[i]->releaseButton((unsigned int*)buffer->interactable);
                }
            }
            Global->keyInput->buttons[BUTTON_MOUSELEFT].is_down = false;
        }
        else {
            for (int i = 0; i < ButtonCount; i++) {
                button[i]->changed = button[0]->is_down != false;
                button[i]->is_down = false;
                button[i]->releaseButton((unsigned int*)buffer->interactable);
            }
        }
        if (Global->keyInput->buttons[BUTTON_BACK].is_down) {
            char* oldword = word;
            int length = strlen(oldword);
            oldword[length - 1] = '\0';
            word = new char[length];
            strcpy_s(word, length, oldword);
            word[length - 1] = '\0';
            word[length - 2] = ' ';
            delete[] oldword;
            typeCursor.x -= 24;
            DrawRect(typeCursor.x, typeCursor.y, typeCursor.x + 24, typeCursor.y + 29, 0xFF000000, buffer, (unsigned int*)buffer->interactable);
            Global->keyInput->buttons[BUTTON_BACK].is_down = false;
        }
        if (Global->keyInput->buttons[BUTTON_CHARACTER].is_down) {
            //our text will be saved in an array of letters. when enter is pressed then we will be on a new line
            // save the old line into a linked list of all our lines and restart our array of letters

            //resize char[] to accomodate more letters each time a letter is recieved
            char* oldword = word;
            int length = strlen(word) + 2;
            word = new char[length];
            strcpy_s(word, length, oldword);
            delete[] oldword;
            word[length - 1] = '\0';
            word[length - 2] = ' ';
            word[length - 3] = (char)(messageReturn & 0x0000FFFF);

            charsToPrint[0] = (char)(messageReturn & 0x0000FFFF);
            charsToPrint[1] = '\0';
            printText(charsToPrint, typeCursor.x, typeCursor.y, buffer, (unsigned int*)buffer->interactable);
            typeCursor.x += 24;
            Global->keyInput->buttons[messageReturn >> 16].is_down = false;
            Global->keyInput->buttons[BUTTON_CHARACTER].is_down = false;
            messageReturn = 0;
        }
        if (Global->keyInput->buttons[BUTTON_RETURN].is_down) {
            typeCursorPrev.y -= 29;
            typeCursor = typeCursorPrev;

            //create a new textLine which will save the new line of text that we made
            textLine* nextNode = text;
            while (nextNode->nextNode != 0) {
                nextNode = nextNode->nextNode;
            }
            nextNode->nextNode = new textLine;
            nextNode->x = typeCursorPrev.x, nextNode->y = typeCursorPrev.y + 29;

            nextNode->text = word;//Here we are handing our pointer to our linked list and then giving our char* word a new pointer to a new array
            word = new char[2];
            word[1] = '\0';
            word[0] = ' ';
            Global->keyInput->buttons[BUTTON_RETURN].is_down = false;
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

        //Do something when a button is pressed
        Range range = button[NewFile]->getRange();
        if (button[NewFile]->is_down) {
            printFileList = false;
            printText((char*)"File name:", 50, 700, buffer, (unsigned int*)buffer->background);
            char* path = new char[50];
            int outcome = createNewFile(window, hdc, buffer, path, Global, (std::string)" ");
            setWindowColor(0x00000AA, buffer, (unsigned int*)buffer->background);
            setWindowColor(0xFF000000, buffer, (unsigned int*)buffer->interactable);
            BMPIN* image = new BMPIN();
            image->Import("Text/System.bmp");
            drawImage(920, 0, 1, image, buffer, (unsigned int*)buffer->background);
            printText((char*)"Cosmic Database", 200, 960, buffer, (unsigned int*)buffer->background);
            if (outcome != 0) {
                printText((char*)"Created New File:", 176, 900, buffer, (unsigned int*)buffer->background);
                printText(path, 380 - 24 * (strlen(path) / 2), 852, buffer, (unsigned int*)buffer->background);
                delete[] path;
                delete image;
            }
        }
        range = button[LoadFile]->getRange();
        if (button[LoadFile]->is_down) {
            printFileList = false;
            //run loadFile
            loadFile(window, hdc, buffer, Global);
            setWindowColor(0x00000AA, buffer, (unsigned int*)buffer->background);
            setWindowColor(0xFF000000, buffer, (unsigned int*)buffer->interactable);

            BMPIN* image = new BMPIN();
            image->Import("Text/System.bmp");
            drawImage(920, 0, 1, image, buffer, (unsigned int*)buffer->background);
            printText((char*)"Cosmic Database", 200, 960, buffer, (unsigned int*)buffer->background);
            delete image;

            button[1]->pressButton((unsigned int*)buffer->interactable);
            button[1]->is_down = false;
        }
        range = button[ListFile]->getRange();
        if (button[ListFile]->is_down) {
            printFileList = true;
            getFiles("./data", ".dat", &fileList, 100, 700);
            DrawRect(100, 300, 900, 800, 0x00000088, buffer, (unsigned int*)buffer->background);
            button[ListFile]->pressButton((unsigned int*)buffer->interactable);
            button[ListFile]->is_down = false;
        }
        range = button[DelFile]->getRange();
        if (button[DelFile]->is_down) {
            printFileList = false;
            printText((char*)"File name:", 50, 700, buffer, (unsigned int*)buffer->background);

            //run deleteFile
            int outcome = deleteFile(window, hdc, buffer, Global);
            setWindowColor(0x00000AA, buffer, (unsigned int*)buffer->background);
            setWindowColor(0xFF000000, buffer, (unsigned int*)buffer->interactable);

            BMPIN* image = new BMPIN();
            image->Import("Text/System.bmp");
            drawImage(920, 0, 1, image, buffer, (unsigned int*)buffer->background);
            printText((char*)"Cosmic Database", 200, 960, buffer, (unsigned int*)buffer->background);
            delete image;
        }
        range = button[Exit]->getRange();
        if (button[Exit]->is_down) {
            Global->runningWindow = false;
        }

        //simulate the Menu screen solar system
        {
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
        }


        //Draw every word in the linked list of words
        textLine* nextNode = text;
        printText(word, typeCursorPrev.x, typeCursorPrev.y, buffer, (unsigned int*)buffer->interactable);
        while (nextNode->nextNode != 0) {
            printText(nextNode->text, nextNode->x, nextNode->y, buffer, (unsigned int*)buffer->interactable);
            nextNode = nextNode->nextNode;
        }

        //for our file list, draw every file
        nextNode = fileList;
        printText(word, typeCursorPrev.x, typeCursorPrev.y, buffer, (unsigned int*)buffer->interactable);
        int yScrollMax = 800 - scroll;
        int yScrollMin = 260 - scroll;
        bool procceedingFilesAreVisible = true;
        if (printFileList) {
            while ((nextNode) && procceedingFilesAreVisible) {
                if (nextNode->y < yScrollMin) { procceedingFilesAreVisible = false; }
                if ((nextNode->y > yScrollMin) && (nextNode->y < yScrollMax) && (procceedingFilesAreVisible)) {
                    printText(nextNode->text, nextNode->x, nextNode->y + scroll, buffer, (unsigned int*)buffer->interactable);
                }
                nextNode = nextNode->nextNode;

            }
        }
        DrawRect(100, 250, 900, 300, 0x000000AA, buffer, (unsigned int*)buffer->interactable);
        DrawRect(100, 800, 900, 850, 0x000000AA, buffer, (unsigned int*)buffer->interactable);
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

        //draw the created image to the screen
        StretchDIBits(hdc, 0, 0, buffer->width, buffer->height, 0, 0, buffer->width, buffer->height, buffer->memory, &buffer->buffer_bitmap_info, DIB_RGB_COLORS, SRCCOPY);
        setWindowColor(0xFF000000, buffer, (unsigned int*)buffer->interactable);

        //get the Frame Rate and print it to the screen
        time.frame_rate = 1 / time.delta_time;
        time.timeKeeperAlpha += time.delta_time;
        if (time.timeKeeperAlpha > 0.2f) {
            DrawRect(5, 960, 150, 990, 0x00000088, buffer, (unsigned int*)buffer->background);
            printNumber(time.frame_rate, 5, 960, buffer, (unsigned int*)buffer->memory);
            time.timeKeeperAlpha = 0;
        }
        //get the time a frame took to render.
        QueryPerformanceCounter(&time.frame_end_time);
        time.delta_time = (float)(time.frame_end_time.QuadPart - time.frame_begin_time.QuadPart) / time.performance_frequency;
        time.frame_begin_time.QuadPart = time.frame_end_time.QuadPart;

    }//end while

    return 0;
}


int proccessMessages(struct Input* keyInput, HWND window, Globals* Global, bufferMem* buffer) {
    //handle all current windows messages
    MSG message;
    int messageReturn = 0;
    int result = 0;
    //get cursor position, convert to coordinates of the window and set our global cursor position;
    POINT p;
    GetCursorPos(&p);
    ScreenToClient(window, &p);
    Global->cursorX = p.x;
    Global->cursorY = buffer->height - p.y;


    while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {

        bool is_down = ((message.lParam & (1 << 31)) == 0);
        for (int i = 0; i < BUTTON_COUNT; i++) {
            keyInput->buttons[i].changed = false;
        }
        switch (message.message) {
            case WM_KEYDOWN:
            case WM_KEYUP: {
                unsigned int vk_code = (unsigned int)message.wParam;
                unsigned int key = vk_code;
                bool is_down = ((message.lParam & (1 << 31)) == 0);
                if (vk_code > 64 && vk_code < 91) {
                    unsigned int key = vk_code;
                    vk_code = 65;
                }
                else if (vk_code > 47 && vk_code < 74) {
                    unsigned int key = vk_code - 17;
                    vk_code = 48;
                }
                else if (vk_code == 32) {
                    unsigned int key = vk_code;
                    vk_code = 65;
                }
                else if (vk_code == 8) {}
                switch (vk_code) {
                case VK_UP: {
                    keyInput->buttons[BUTTON_UP].is_down = is_down;
                    keyInput->buttons[BUTTON_UP].changed = true;
                }break;
                case VK_DOWN: {
                    keyInput->buttons[BUTTON_DOWN].is_down = is_down;
                    keyInput->buttons[BUTTON_DOWN].changed = true;

                }break;
                case VK_LEFT: {
                    keyInput->buttons[BUTTON_LEFT].is_down = is_down;
                    keyInput->buttons[BUTTON_LEFT].changed = true;
                }break;
                case 65: {
                    keyInput->buttons[5].changed = keyInput->buttons[5].is_down != is_down;
                    keyInput->buttons[5].is_down = is_down;
                    keyInput->buttons[BUTTON_CHARACTER].is_down = is_down;
                    messageReturn = key + (5 << 16);
                }break;
                case 48: {
                    keyInput->buttons[31].changed = keyInput->buttons[31].is_down != is_down;
                    keyInput->buttons[31].is_down = is_down;
                    keyInput->buttons[BUTTON_CHARACTER].is_down = is_down;
                    messageReturn = key + (31 << 16);

                }break;
                case 190: {
                    keyInput->buttons[BUTTON_PERIOD].changed = keyInput->buttons[BUTTON_PERIOD].is_down != is_down;
                    keyInput->buttons[BUTTON_PERIOD].is_down = is_down;
                    keyInput->buttons[BUTTON_CHARACTER].is_down = is_down;
                    messageReturn = 46 + (BUTTON_PERIOD << 16);
                }
                case VK_SPACE: {

                }break;
                case VK_RETURN: {
                    keyInput->buttons[BUTTON_RETURN].changed = keyInput->buttons[5].is_down != is_down;
                    keyInput->buttons[BUTTON_RETURN].is_down = is_down;
                }break;
                case VK_BACK: {
                    keyInput->buttons[BUTTON_BACK].changed = keyInput->buttons[5].is_down != is_down;
                    keyInput->buttons[BUTTON_BACK].is_down = is_down;
                    return messageReturn;
                }break;
                case 187: {
                    if (keyInput->buttons[BUTTON_SHIFT].is_down == true) {
                        keyInput->buttons[BUTTON_UNDERSCORE].is_down = is_down;
                        keyInput->buttons[BUTTON_CHARACTER].is_down = is_down;
                        keyInput->buttons[BUTTON_UNDERSCORE].changed = true;
                        messageReturn = 95 + (BUTTON_UNDERSCORE << 16);
                    }
                    else {
                        keyInput->buttons[BUTTON_SUBTRACT].is_down = is_down;
                        keyInput->buttons[BUTTON_CHARACTER].is_down = is_down;
                        keyInput->buttons[BUTTON_SUBTRACT].changed = true;
                        messageReturn = 45 + (BUTTON_SUBTRACT << 16);
                    }
                    return messageReturn;
                }break;
                case VK_SHIFT: {
                    keyInput->buttons[BUTTON_SHIFT].is_down = is_down;
                    keyInput->buttons[BUTTON_SHIFT].changed = true;

                }break;
                default: {
                    TranslateMessage(&message);
                    DispatchMessage(&message);
                }
                }
            }break;
            case WM_LBUTTONUP: {
                Global->keyInput->buttons[BUTTON_MOUSELEFT].changed = (Global->keyInput->buttons[BUTTON_MOUSELEFT].is_down != is_down);
                Global->keyInput->buttons[BUTTON_MOUSELEFT].is_down = false;
            }return messageReturn;
            case WM_LBUTTONDOWN: {
                Global->keyInput->buttons[BUTTON_MOUSELEFT].changed = (Global->keyInput->buttons[BUTTON_MOUSELEFT].is_down != is_down);
                Global->keyInput->buttons[BUTTON_MOUSELEFT].is_down = true;
                POINT p;
                GetCursorPos(&p);
                ScreenToClient(window, &p);
                p.x;
                p.y;
                Global->cursorX = p.x;
                Global->cursorY = buffer->height - p.y;
            }return messageReturn;
            case WM_CLOSE:
            case WM_DESTROY: {
                Global->runningWindow = false;
            } break;
            case WM_SIZE: {

                //if the window memory == 0 then it hasn't been initialized; Initialize window memory.
                //This if statement is necessary to prevent memory buffers from being deleted upon minimizing the window.
                if (buffer->memory == 0) {
                    //get window dimensins
                    RECT rect;
                    GetClientRect(window, &rect);
                    buffer->width = rect.right - rect.left;
                    buffer->height = rect.bottom - rect.top;


                    int buffer_size = buffer->width * buffer->height * sizeof(unsigned int);

                    if (buffer->memory) VirtualFree(buffer->memory, 0, MEM_RELEASE);
                    buffer->memory = VirtualAlloc(0, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
                    buffer->buffer_bitmap_info.bmiHeader.biSize = sizeof(buffer->buffer_bitmap_info.bmiHeader);
                    buffer->buffer_bitmap_info.bmiHeader.biWidth = buffer->width;
                    buffer->buffer_bitmap_info.bmiHeader.biHeight = buffer->height;
                    buffer->buffer_bitmap_info.bmiHeader.biPlanes = 1;
                    buffer->buffer_bitmap_info.bmiHeader.biBitCount = 32;
                    buffer->buffer_bitmap_info.bmiHeader.biCompression = BI_RGB;

                    //allocate memory for buffer_background
                    if (buffer->background) VirtualFree(buffer->background, 0, MEM_RELEASE);
                    buffer->background = VirtualAlloc(0, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

                    //allocate memory for buffer_interactable
                    if (buffer->interactable) VirtualFree(buffer->interactable, 0, MEM_RELEASE);
                    buffer->interactable = VirtualAlloc(0, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);


                    setWindowColor(0x00000AA, buffer, (unsigned int*)buffer->background);
                    setWindowColor(0xFF000000, buffer, (unsigned int*)buffer->interactable);
                    printText((char*)"Cosmic Database", 200, 960, buffer, (unsigned int*)buffer->background);
                    BMPIN* image = new BMPIN();
                    image->Import("Text/System.bmp");
                    drawImage(920, 0, 1, image, buffer, (unsigned int*)buffer->background);
                    delete image;
                }
            }
            case WM_MOUSEWHEEL: {
                keyInput->buttons[BUTTON_WHEEL].changed = true;
                keyInput->buttons[BUTTON_WHEEL].is_down = true;

                messageReturn = message.wParam >> 16;
                return messageReturn;
            }
        }
        if (message.message == 15) {
            return messageReturn;
        }
    }
    return messageReturn;
}
