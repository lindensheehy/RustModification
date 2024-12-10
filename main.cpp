#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <stdio.h>
#include <ObjIdl.h>
#include <gdiplus.h>

#include "include/log.h"
#include "include/utility.h"
#include "include/aob.h"
#include "include/process.h"

using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")



// Global Variables
Process* process;
Cheat* cheat;

CharacterList* characters;
int8bytes playerAddress = 0;

HWND window;

RECT windowDim;


// Early Declaration
void draw(HWND hwnd, HDC hdc);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    switch (uMsg) {

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        case WM_PAINT:
            draw(hwnd, GetDC(hwnd));
            ShowWindow(hwnd, SW_SHOW);
            UpdateWindow(hwnd);
            break;

    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);

}

HWND CreateTransparentWindow() {

    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;

    //Initialize GDI+
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    const char CLASS_NAME[] = "TransparentWindowClass";     // Define window class name
    HINSTANCE hinst = GetModuleHandle(NULL);                // Get the current application instance

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;  // Window procedure
    wc.hInstance = hinst;
    wc.lpszClassName = CLASS_NAME;

    // Register window class, throw error on fail
    if (!RegisterClass(&wc)) {
        MessageBox(NULL, "Window class registration failed!", "Error", MB_OK);
        exit(1);
    }

    // Make fullscreen on primary moniter
    POINT pt = { 0, 0 };
    HMONITOR hMonitor = MonitorFromPoint(pt, MONITOR_DEFAULTTOPRIMARY);

    MONITORINFO monitorInfo = { sizeof(MONITORINFO) };
    if (!GetMonitorInfo(hMonitor, &monitorInfo)) {
        MessageBox(NULL, "GetMoniterInfo Failed!", "Error", MB_OK);
        exit(1);
    }

    // Get the monitor's working area
    windowDim = monitorInfo.rcMonitor;

    // Create the window
    HWND hwnd = CreateWindowEx(
        WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TRANSPARENT,     // transparent | always on top | not clickable
        CLASS_NAME,
        "Overlay",                          // Window title
        WS_OVERLAPPEDWINDOW,                // Default window style
        windowDim.left, windowDim.top,                               // Position
        windowDim.right - windowDim.left, windowDim.bottom - windowDim.top,                           // Size
        NULL, 
        NULL, 
        hinst, 
        NULL
    );

    if (hwnd == NULL) {
        MessageBox(NULL, "Window creation failed!", "Error", MB_OK);
        exit(1);
    }

    // Draw content
    draw(hwnd, GetDC(hwnd));

    // Show and update window
    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    return hwnd;

}

void draw(HWND hwnd, HDC hdc) {
    
    FontFamily  fontFamily(L"Times New Roman");
    Font        font(&fontFamily, 32, FontStyleRegular, UnitPixel);
    PointF      pointF(30.0f, 10.0f);
    SolidBrush  solidBrush(Color(255, 0, 0, 0));

    Bitmap softwareBitmap(
        windowDim.right - windowDim.left, 
        windowDim.bottom - windowDim.top, 
        PixelFormat32bppARGB
    );
    Graphics g(&softwareBitmap);

    g.Clear(Gdiplus::Color(0, 0, 0, 0));  // 30: alpha value 



    /*   Drawing Logic   */

    // Update characters list
    if (characters != nullptr) delete characters;
    characters = cheat->getList();
    characters->setPlayer(playerAddress);
    characters->projectAll();

    // Define pen to use
    Pen rectanglePen(Color(255, 255, 20, 147), 1);

    // Iterator over characters
    Character* current;
    for (int i = 0; i < characters->size; i++) {

        current = characters->getCharacter(i);

        if (current == nullptr) break;

        int drawposx = (int) current->projected->x - 50;
        int drawposy = (int) current->projected->y + (current->projected->z / 7);

        int sizex = (int) (500 / current->projected->z);
        int sizey = (int) (1000 / current->projected->z);

        // Draw esp box
        g.DrawRectangle(
            &rectanglePen, 
            drawposx,           // pos x
            drawposy,           // pos y
            sizex,              // size x
            sizey               // size y
        );

    }



    HBITMAP bmp;
    softwareBitmap.GetHBITMAP(Color(0, 0, 0, 0), &bmp);

    HDC memdc = CreateCompatibleDC(hdc);
    HGDIOBJ original = SelectObject(memdc, bmp);


    BLENDFUNCTION blend = { 0 };
    blend.BlendOp = AC_SRC_OVER;
    blend.SourceConstantAlpha = 255;
    blend.AlphaFormat = AC_SRC_ALPHA;
    POINT ptLocation = { 0, 0 };
    SIZE szWnd = { windowDim.right - windowDim.left, windowDim.bottom - windowDim.top };
    POINT ptSrc = { 0, 0 };
    UpdateLayeredWindow(hwnd, hdc, &ptLocation, &szWnd, memdc, &ptSrc, 0, &blend, ULW_ALPHA);
    SelectObject(hdc, original);

    DeleteObject(bmp);
    DeleteObject(memdc);

}


// Clean up on close
BOOL WINAPI ConsoleHandler(DWORD event) {

    if (event == CTRL_CLOSE_EVENT || event == CTRL_C_EVENT || event == CTRL_BREAK_EVENT) {

        std::cout << "Cleaning up resources before closing...\n";
        std::cout << "Exiting program and cleaning up resources...\n";

        delete cheat;
        delete process;

        std::cout << "Closed Handle\n";

        system("pause");

        return TRUE; // Signal handled
    }

    return FALSE; // Let the system handle it

}

int setup() {

    logInit("log.txt");
    logClear();

    process = new Process("RustClient.exe");
    if (process->processId == -1) return -1;

    logWrite("Successfully attached to process!", true);
    std::cout << "Successfully attached to process!" << std::endl;

    cheat = new Cheat(process);
    if (cheat->addr.allocatedMemory == 0) return -1;

    logWrite("Successfully instantiated Cheat!", true);
    std::cout << "Successfully instantiated Cheat!" << std::endl;

    cheat->toggleHook();

    Sleep(100);

    characters = cheat->getList();
    characters->setPlayer(playerAddress);

    return 0;

}

void doInput(const std::string& input, bool* quit) {

    std::string command, arg1, arg2;
    int currentArg = 0; // 0 means it's taking the command, after that it's for arguments

    // Parse the input string into command and arguments
    for (char c : input) {
        if (c == ' ') {
            currentArg++;
            continue;
        }

        switch (currentArg) {
        case 0:
            command.append(1, c);
            break;
        case 1:
            arg1.append(1, c);
            break;
        case 2:
            arg2.append(1, c);
            break;
        }
    }

    if (command == "hook") {

        cheat->toggleHook();
        return;

    }

    if (command == "print") {

        logNewLine();
        logWrite("Printing character list Entries...", true);

        if (!characters) {
            logWrite("Character list is null.", true);
            return;
        }

        for (int i = 0; i < characters->size; i++) {

            Character* character = characters->getCharacter(i);

            if (!character) break;

            // Log index label
            logWrite(std::to_string(i + 1) + ":", true);

            // Log base address
            logWrite("  Base Address -> ", false);
            logWriteHex(character->address, true);

            // Log position
            logWrite("  Position -> ", false);
            logWrite(character->position->x, false);
            logWrite(", ", false);
            logWrite(character->position->y, false);
            logWrite(", ", false);
            logWrite(character->position->z, true);

            // Log facing vector
            logWrite("  Facing -> ", false);
            logWrite(character->facing->x, false);
            logWrite(", ", false);
            logWrite(character->facing->y, false);
            logWrite(", ", false);
            logWrite(character->facing->z, true);

        }

        logNewLine();
        std::cout << "Logged character list entries!\n";

        return;

    }

    if (command == "get") {

        if (characters != nullptr) delete characters;

        characters = cheat->getList();
        characters->setPlayer(playerAddress);

        return;

    }

    std::cout << "Unknown command: " << command << "\n";
    return;

}

int main() {

    // Force clean up on close
    SetConsoleCtrlHandler(ConsoleHandler, TRUE);

    int setupStatus = setup();
    if (setupStatus == -1) return -1;

    std::cout << "Looking for player...\n";

    // Locate player in character list. Cant really draw anything until this happens
    while (playerAddress == 0) {

        Character* current;

        for (int i = 0; i < characters->size; i++) {

            current = characters->getCharacter(i);

            if (current == nullptr) break;

            if ( !(current->facing->is(0, 0, 0)) ) {

                logWrite("Found Player!", true);
                std::cout << "Found Player!\n";

                playerAddress = current->address;
                break;

            }

        }

        Sleep(200);

        characters = cheat->getList();

    }

    std::cout << "Player found!\n\n";

    std::cout << "Creating overlay\n";


    // Main Loop
    HWND hwnd = CreateTransparentWindow();

    // Message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {

        TranslateMessage(&msg);
        DispatchMessage(&msg);

        // Invalidate to trigger repaint every frame
        InvalidateRect(hwnd, NULL, TRUE);

    }


    std::cout << "Exiting program and cleaning up resources...\n";
    delete cheat;
    delete process;
    std::cout << "Closed Handle\n";
    system("pause");
    return 0;

}
