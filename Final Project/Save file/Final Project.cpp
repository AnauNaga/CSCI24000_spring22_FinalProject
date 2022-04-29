//windows includes
#include "windows.h"
#include "framework.h"
#include "tchar.h"//used to create a LPCWSTR variable
#include "math.h"

#include "string"
#include "iostream"
#include "math.h"
#include "stdio.h"
#include "fileSystem"
#include "sstream"
//local includes
#include "Button.h"
#include "KeyTracker.cpp"
#include "BMPIO.h"
#include "BMPOUT.h"
#include "BMPIN.h"
#include "GuiUtilites.h"

int proccessMessages(Input* keyInput, int* cursorX, int* cursorY, HWND window);

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);


int setPixelColor(int X, int Y, unsigned int color, struct bufferMem buffer); 

//windows buffer info
struct bufferMem buffer = {};

//global cursor position
int cursorX;
int cursorY;

Button* button[10];

//global status for all buttons
struct Input keyInput = {};
Input* keyInputPtr = &keyInput;
//global program run status. (This is for the window, but if the window closes then the program is done)
bool runningWindow = true;


//Read messages sent by windows and do something based on them.
LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;

    //Windows likes to store information in each individual bit of a byte. Get the bit reguarding if a key is pressed down from lparam.
    bool is_down = ((lParam & (1 << 31)) == 0);

    //do something based off the message read.
    switch (uMsg) {
    case WM_CLOSE:
    case WM_DESTROY: {
        runningWindow = false;
    } break;
    case WM_SIZE: {
        
        //if the window memory == 0 then it hasn't been initialized; Initialize window memory.
        //This if statement is necessary to prevent memory buffers from being deleted upon minimizing the window.
        if (buffer.memory == 0) {
            //get window dimensins
            RECT rect;
            GetClientRect(hwnd, &rect);
            buffer.width = rect.right - rect.left;
            buffer.height = rect.bottom - rect.top;


            int buffer_size = buffer.width * buffer.height * sizeof(unsigned int);

            if (buffer.memory) VirtualFree(buffer.memory, 0, MEM_RELEASE);
            buffer.memory = VirtualAlloc(0, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
            buffer.buffer_bitmap_info.bmiHeader.biSize = sizeof(buffer.buffer_bitmap_info.bmiHeader);
            buffer.buffer_bitmap_info.bmiHeader.biWidth = buffer.width;
            buffer.buffer_bitmap_info.bmiHeader.biHeight = buffer.height;
            buffer.buffer_bitmap_info.bmiHeader.biPlanes = 1;
            buffer.buffer_bitmap_info.bmiHeader.biBitCount = 32;
            buffer.buffer_bitmap_info.bmiHeader.biCompression = BI_RGB;

            //allocate memory for buffer_background
            if (buffer.background) VirtualFree(buffer.background, 0, MEM_RELEASE);
            buffer.background = VirtualAlloc(0, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

            //allocate memory for buffer_interactable
            if (buffer.interactable) VirtualFree(buffer.interactable, 0, MEM_RELEASE);
            buffer.interactable = VirtualAlloc(0, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);


            setWindowColor(0xFF000000, buffer, (unsigned int*)buffer.interactable);
            setWindowColor(0x0000FF00, buffer, (unsigned int*)buffer.background);
            for (int x = 820; x < buffer.width; x++) {
                for (int y = 0; y < 1000; y++) {
                    setPixelColor(x, y, 0x00000000, buffer, (unsigned int*)buffer.background);

                }
            }
            
        }
    }
    case WM_LBUTTONUP: {
        keyInput.buttons[BUTTON_MOUSELEFT].is_down = false;
        keyInput.buttons[BUTTON_MOUSELEFT].changed = false;
        return result;
    }
    case WM_LBUTTONDOWN: {
        keyInput.buttons[BUTTON_MOUSELEFT].changed = (keyInput.buttons[BUTTON_MOUSELEFT].is_down != is_down);
        keyInput.buttons[BUTTON_MOUSELEFT].is_down = true;
        POINT p;
        GetCursorPos(&p);
        ScreenToClient(hwnd, &p);
        p.x;
        p.y;
        cursorX = p.x;
        cursorY = buffer.height - p.y;
    }return result;
    default: {

        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    }
    return result;
}



//run the window
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
   
    //create a window and set its starting settings
    WNDCLASS window_class = {};
    window_class.style = CS_HREDRAW | CS_VREDRAW;
    window_class.lpszClassName = L"Game Class Window";
    window_class.lpfnWndProc = window_callback;
    RegisterClass(&window_class);
    HWND window = CreateWindow(window_class.lpszClassName, L"My Project", WS_OVERLAPPED | WS_VISIBLE | WS_MINIMIZEBOX | WS_SYSMENU, 0,0, 1936, 1040, 0, 0, hInstance, 0);
    HDC hdc = GetDC(window);


    timeInfo time;
    QueryPerformanceCounter(&time.frame_begin_time);
    QueryPerformanceFrequency(&time.perf);
    //performance_frequency is the processor clock speed 
    time.performance_frequency = (float)time.perf.QuadPart;

    
    POINT typeCursor = {0,0};
    POINT typeCursorPrev = {0,0};
    char charsToPrint[2];

    
    //preload mars and Planet images
    BMPIN* Mars = new BMPIN();
    Mars->Import("text/Mars.bmp", 0x00000000);
    BMPIN* Planet = new BMPIN();
    Planet->Import("text/Planet.bmp", 0x00000000);
    int Planetx = 1425;
    int Planety = 500;
    double PlanetTheta = 0;
    double moonTheta = 0;
    int messageReturn;

    Letter** LettersPtr = new Letter*[5];
    LettersPtr[0] = new Letter;
    *LettersPtr[0] = { 'A', 10, 10 };
    LettersPtr[1] = new Letter;
    *LettersPtr[0] = { 'A', 10, 10 };
    printText((char*)"Cosmic Database", LettersPtr, 200, 960, buffer, (unsigned int*)buffer.background);
    for (int i = 0; i < sizeof(button) / sizeof(button[0]); i++) {
        button[i] = new Button();
    }
    delete button[0];
    button[0] = new Button(100, 100, (std::string)"Create New File", (unsigned int*)buffer.interactable, buffer.width);
    delete button[1];
    button[1] = new Button(100, 149, (std::string)"Load File", (unsigned int*)buffer.interactable, buffer.width);
    delete button[2];
    button[2] = new Button(100, 197, (std::string)"List Files", (unsigned int*)buffer.interactable, buffer.width);
    SetCursor(LoadCursor(NULL, IDC_ARROW));

    float scroll = 50.0f;
    while (runningWindow) {
        

        messageReturn = proccessMessages(keyInputPtr, &cursorX, &cursorY, window);

        

        if (keyInput.buttons[BUTTON_MOUSELEFT].is_down) {
            Range range = button[0]->getRange();
            Range range2 = button[1]->getRange();
            Range range3 = button[2]->getRange();
            typeCursor = { 24 * (cursorX / 24),29 * (cursorY / 29) };
            typeCursorPrev = typeCursor;
            range = button[0]->getRange();
            range2 = button[1]->getRange();
            if ((cursorX < range.xMax) && (cursorX > range.xMin) && (cursorY < range.yMax) && (cursorY > range.yMin)) {
                button[0]->changed = button[0]->is_down != true;
                button[0]->is_down = true;
            }
            else if ((cursorX < range2.xMax) && (cursorX > range2.xMin) && (cursorY < range2.yMax) && (cursorY > range2.yMin)) {
                button[1]->changed = button[1]->is_down != true;
                button[1]->is_down = true;
            }
            else if ((cursorX < range3.xMax) && (cursorX > range3.xMin) && (cursorY < range3.yMax) && (cursorY > range3.yMin)) {
                button[2]->changed = button[2]->is_down != true;
                button[2]->is_down = true;
            }

        }else {
            button[0]->changed = button[0]->is_down != false;
            button[0]->is_down = false;
            button[1]->changed = button[1]->is_down != false;
            button[1]->is_down = false;
            button[2]->changed = button[2]->is_down != false;
            button[2]->is_down = false;
        }

        if (keyInput.buttons[5].is_down || keyInput.buttons[31].is_down) {
            charsToPrint[0] = (char)messageReturn;
            charsToPrint[1] = '\0';
            printText(charsToPrint, LettersPtr, typeCursor.x ,typeCursor.y,buffer,(unsigned int*)buffer.background);
            typeCursor.x += 24;
            keyInput.buttons[5].is_down = false;
            keyInput.buttons[31].is_down = false;
        }
        if (keyInput.buttons[BUTTON_RETURN].is_down) {
            typeCursorPrev.y -= 29;
            typeCursor = typeCursorPrev;
            keyInput.buttons[BUTTON_RETURN].is_down = false;
        }
        
        
        //get the Frame Rate and print it to the screen
        time.frame_rate = 1 / time.delta_time;
        time.seconds_counter += time.delta_time;
        if (time.seconds_counter > 0.2f) {
            DrawRect(5, 960, 150, 990, 0x0000FF00, buffer, (unsigned int*)buffer.background);
            printNumber(time.frame_rate, 5, 960, buffer, (unsigned int*)buffer.memory);
            time.seconds_counter = 0;
        }

        //combine all the bitmap layers
        unsigned int* background = (unsigned int*)buffer.background;
        unsigned int* memory = (unsigned int*)buffer.memory;
        unsigned int* interactable = (unsigned int*)buffer.interactable;
        for (int x = 0; x < buffer.width; x++) {
            for (int y = 0; y < buffer.height; y++) {
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

        if (keyInput.buttons[BUTTON_WHEEL].is_down) {
            int value = ((short signed int)messageReturn);
            scroll = scroll + ( value / 30);
            DrawRect(450, 10, 690, 500, 0x0000FF00, buffer, (unsigned int*)buffer.background);
            printNumber(123, 500, (int)scroll, buffer, (unsigned int*)buffer.background);
            keyInput.buttons[BUTTON_WHEEL].is_down = false;
        }

        setWindowColor(0xFF000000, buffer, (unsigned int*)buffer.interactable);
        Range range = button[0]->getRange();
        if (button[0]->is_down) { 
            button[0]->pressButton((unsigned int*)buffer.interactable);
            printText((char*)"File name:", LettersPtr, 50,700, buffer,(unsigned int*)buffer.background);
            typeCursor.x = 290;
            typeCursor.y = 700;
            button[0]->is_down = false;
        }
        else { button[0]->releaseButton((unsigned int*)buffer.interactable);}
        range = button[1]->getRange();
        if (button[1]->is_down) { button[1]->pressButton((unsigned int*)buffer.interactable); }
        else { button[1]->releaseButton((unsigned int*)buffer.interactable); }
        range = button[2]->getRange();
        if (button[2]->is_down) { 
            std::string path = "./Bin";
            std::string directory;
            std::stringstream sstream;
            int line = 500;
            for (const auto& entry : std::filesystem::directory_iterator(path)) {

                sstream << entry.path() << std::endl;
                std::getline(sstream, directory);
                char* c = const_cast<char*>(directory.c_str());
                printText(c, LettersPtr, 10, line, buffer, (unsigned int*)buffer.background);
                line -= 48;
            }

            button[2]->pressButton((unsigned int*)buffer.interactable); 
            button[2]->is_down = false;
        }
        else { button[2]->releaseButton((unsigned int*)buffer.interactable); }

        PlanetTheta = PlanetTheta + (0.5f * time.delta_time);
        moonTheta = moonTheta + (1.5f * time.delta_time);
        if (PlanetTheta > 6.283184) { PlanetTheta = 0; }
        if (moonTheta > 6.283184) { moonTheta = 0; }


        int PlanX = Planetx + 20 * cos(PlanetTheta + 3.14f);
        int PlanY = Planety + 2 * sin(PlanetTheta + 3.14f);
        int Planz = PlanY;
        int Plax = Planetx + 400 * cos(PlanetTheta);
        int Play = Planety + 40 * sin(PlanetTheta);
        int Plaz = Play;
        int moox = Plax + 100 * cos(moonTheta);
        int mooy = Play + 10 * sin(moonTheta);
        int mooz = mooy;
        float plascale = 100/(float)(Play-350);
        float mooscale = 60/(float)(mooy-350);
        float planscale = 500 / (float)(PlanY);
        Plax -= plascale * 100 / 2;
        Play -= plascale * 100 / 2;
        moox -= mooscale * 100 / 2;
        mooy -= mooscale * 100 / 2;
        PlanX -= planscale * 100 / 2;
        PlanY -= planscale * 100 / 2;
        if (Plaz > Planz) {
            if (mooz > Plaz) {
                drawImage(moox, mooy, mooscale, Mars, buffer, (unsigned int*)buffer.interactable);
                drawImage(Plax, Play, plascale, Planet, buffer, (unsigned int*)buffer.interactable);
            }else {
                drawImage(Plax, Play, plascale, Planet, buffer, (unsigned int*)buffer.interactable);
                drawImage(moox, mooy, mooscale, Mars, buffer, (unsigned int*)buffer.interactable);
            }
            drawImage(PlanX, PlanY, planscale, Mars, buffer, (unsigned int*)buffer.interactable);
        }
        else {
            
            drawImage(PlanX, PlanY, planscale, Mars, buffer, (unsigned int*)buffer.interactable);
            if (mooz > Plaz) {
                drawImage(moox, mooy, mooscale, Mars, buffer, (unsigned int*)buffer.interactable);
                drawImage(Plax, Play, plascale, Planet, buffer, (unsigned int*)buffer.interactable);
            }
            else {
                drawImage(Plax, Play, plascale, Planet, buffer, (unsigned int*)buffer.interactable);
                drawImage(moox, mooy, mooscale, Mars, buffer, (unsigned int*)buffer.interactable);
            }
        }

        

        //draw the created image to the screen
        StretchDIBits(hdc, 0, 0, buffer.width, buffer.height, 0, 0, buffer.width, buffer.height, buffer.memory, &buffer.buffer_bitmap_info, DIB_RGB_COLORS, SRCCOPY);


         //get the time a frame took to render.
        QueryPerformanceCounter(&time.frame_end_time);
        time.delta_time = (float)(time.frame_end_time.QuadPart - time.frame_begin_time.QuadPart) / time.performance_frequency;
        time.frame_begin_time.QuadPart = time.frame_end_time.QuadPart;
    }//end while
}//end winMain




int proccessMessages(struct Input* keyInput, int* cursorX, int* cursorY, HWND window) {
    //handle all current windows messages
    MSG message;
    int messageReturn = 0;
    while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
        //get cursor position, convert to coordinates of the window and set our global cursor position;

    //coordinates of the cursor
        POINT p;

        GetCursorPos(&p);
        ScreenToClient(message.hwnd, &p);
        *cursorX = p.x;
        *cursorY = buffer.height - p.y;
        for (int i = 0; i < BUTTON_COUNT; i++) {
            keyInput->buttons[i].changed = false;
        }
        if ((WM_KEYDOWN == message.message) || (WM_KEYUP == message.message)) {
            unsigned int vk_code = (unsigned int)message.wParam;
            unsigned int key = vk_code;
            bool is_down = ((message.lParam & (1 << 31)) == 0);
            if (vk_code > 64 && vk_code < 91) {
                unsigned int key = vk_code;
                vk_code = 65;
            }else if (vk_code > 47 && vk_code < 74) {
                unsigned int key = vk_code - 17;
                vk_code = 48;
            }else if (vk_code == 32) {
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
                messageReturn = key;
            }break;
            case 48: {
                keyInput->buttons[31].changed = keyInput->buttons[31].is_down != is_down;
                keyInput->buttons[31].is_down = is_down;
                messageReturn = key;

            }break;
            case VK_SPACE: {

            }break;
            case VK_RETURN: {
                keyInput->buttons[BUTTON_RETURN].changed = keyInput->buttons[5].is_down != is_down;
                keyInput->buttons[BUTTON_RETURN].is_down = is_down;
            }
            case VK_BACK: {
                return messageReturn;
            }
            default: {
                TranslateMessage(&message);
                DispatchMessage(&message);
            }
            }
        }else if (message.message == WM_MOUSEWHEEL) {
            keyInput->buttons[BUTTON_WHEEL].changed = true;
            keyInput->buttons[BUTTON_WHEEL].is_down = true;

            messageReturn = message.wParam >> 16;
            return messageReturn;
        }
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
    return messageReturn;
}
