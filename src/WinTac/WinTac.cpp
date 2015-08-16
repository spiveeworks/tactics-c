
#include <string>
#include <sstream>

#include "..\env.cpp"
    //Data structures for game objects

#include <windows.h>
    //Windows interface tools

#include "screen.cpp"
    //Data structures and functionality for windows API handling env.cpp objects

#include "game.h"
	//to be cleaned back into things like input.h or something

#include <stdlib.h>
    //Standard C and C++ tools used in this implementation

HWND WindowRoot (HINSTANCE hInstance, WNDCLASSEX &wc);


const char g_szClassName[] = "myWindowClass";
const int ID_TIMER = 1;


//LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

GameInstance game;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {	
        case WM_CREATE:
        {
            //srand(GetCurrentTime());
			game.input = defaultInputs();
			game.playercam.rec.dir.west = 50;
			game.playercam.rec.dir.north = 50;
			
			game.playercam.rec.dir.east = 550;
			game.playercam.rec.dir.south = 550;
			
			
            
            UINT ret;
			ret = SetTimer(hwnd, ID_TIMER, 50, NULL);
			if(ret == 0)
				MessageBox(hwnd, "Could not SetTimer()!", "Error", MB_OK | MB_ICONEXCLAMATION);
        }
        break;
        case WM_PAINT:
        {
            RECT rcClient;
            screenPaintHandler screenH;
            
            GetClientRect(hwnd, &rcClient);
			screenH.open(hwnd);
			
            FillRect(screenH.get(), &rcClient, (HBRUSH)GetStockObject(WHITE_BRUSH));
            //FillRect(screenH.dc, &gameClient, (HBRUSH)GetStockObject(BLACK_BRUSH));
			render(screenH.get(), rcClient, game.playercam);
            
			screenH.close();
        }
        break;
        case WM_TIMER:
        {
			game.update();
			RECT rcClient;
			screenHandler screenH;
			
			GetClientRect(hwnd, &rcClient);
			screenH.open(hwnd);
			
			render (screenH.get(), rcClient, game.playercam);
			
			screenH.close();
        }
        break;
        
        
        
        
        case WM_KEYDOWN: 
            game.input.updateKeyState(wParam, true);//no check because no other keys are relevant yet;
		break;
		case WM_KEYUP: 
            game.input.updateKeyState(wParam, false);//no check because no other keys are relevant yet;
		break;
        
        
        case WM_CLOSE:
            DestroyWindow(hwnd);
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
        break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;
	
	hwnd = WindowRoot(hInstance, wc);

	if (hwnd == NULL)
		return 0;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}


//I pretty much got all of this from a tutorial. 
		//I just moved it into this function and dealt with return values >.>
HWND WindowRoot (HINSTANCE hInstance, WNDCLASSEX &wc) {
//initialised constants and creates a window, passing its handle back
	HWND hwnd;
	
    wc.cbSize		 = sizeof(WNDCLASSEX);
    wc.style		 = 0;
    wc.lpfnWndProc	 = WndProc;
    wc.cbClsExtra	 = 0;
    wc.cbWndExtra	 = 0;
    wc.hInstance	 = hInstance;
    wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return NULL;
    }

    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        "An Animation Program",
        WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME,
        CW_USEDEFAULT, CW_USEDEFAULT, 616, 630,
        NULL, NULL, hInstance, NULL);

    if(hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return NULL;
    }
	return hwnd;
}