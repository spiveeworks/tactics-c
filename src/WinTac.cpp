
#include <string>
#include <sstream>

#include "env.cpp"
    //Data structures for game objects

#include <windows.h>
    //Windows interface tools

#include "screen.cpp"
    //Data structures and functionality for windows API handling env.cpp objects

#include <stdlib.h>
    //Standard C and C++ tools used in this implementation

const char g_szClassName[] = "myWindowClass";
const int ID_TIMER = 1;


bool kW, kA, kS, kD;
double kQ, kE;


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {	
        case WM_CREATE:
        {
            init();
            
            UINT ret;
			ret = SetTimer(hwnd, ID_TIMER, 50, NULL);
			if(ret == 0)
				MessageBox(hwnd, "Could not SetTimer()!", "Error", MB_OK | MB_ICONEXCLAMATION);
        }
        break;
        case WM_PAINT:
        {
            RECT rcClient;
            GetClientRect(hwnd, &rcClient);
            
            PAINTSTRUCT ps;
            dcStruct dat;
			dat.dc = BeginPaint(hwnd, &ps);
            FillRect(dat.dc, &rcClient, (HBRUSH)GetStockObject(WHITE_BRUSH));
            //FillRect(dat.dc, &gameClient, (HBRUSH)GetStockObject(BLACK_BRUSH));
            
			render(dat, gameClient);

            EndPaint(hwnd, &ps);
        }
        break;
        case WM_TIMER:
        {
            SCG::vector move (0.0,0.0);
			if (kW) move.y++;
			if (kS) move.y--;
			if (kA) move.x--;
			if (kD) move.x++;
            if (move.square() != 0) 
                move /= move.magnitude();
			//move *= 0.001;
			playercam.focus += move;
			playercam.tilt += (kQ + kE)/20;
            
            dcStruct dat;
			dat.dc = GetDC(hwnd);
			
			RECT rcClient;
			GetClientRect(hwnd, &rcClient);

			/*
			POINT coord;
			GetCursorPos (&coord);
			ScreenToClient (hwnd, &coord);
			current.vertices.push_back (playercam.screen_to_world (coord));
			*/

    std::stringstream s;
    s << long(256 * kQ) << ", " << long(256 * kE) << "; "<< long(100 * playercam.tilt);
    std::string sstr = s.str();
    DrawText(dat.dc, TEXT(sstr.c_str()),sstr.length(), &rcClient, DT_TOP|DT_LEFT);
			
			render (dat, gameClient);
			
			ReleaseDC(hwnd, dat.dc);
			
			//PostMessage (hwnd, WM_RECURSE, 0, GetCurrentTime ());
        }
        break;
        
        
        
        
        case WM_KEYDOWN: 
			switch (wParam) 
            { 
                case 0x57: //W
					kW = true;
				break; 
                case 0x41: //A
					kA = true;
				break; 
                case 0x53: //S
					kS = true;
				break; 
                case 0x44: //D
					kD = true;
				break; 
                case 0x51: //Q
					kQ = 1.0;
				break; 
                case 0x45: //E
					kE = -1.0;
				break; 
            } 
		break;
		case WM_KEYUP: 
			switch (wParam) 
            { 
                case 0x57: //W
					kW = false;
				break; 
                case 0x41: //A
					kA = false;
				break; 
                case 0x53: //S
					kS = false;
				break; 
                case 0x44: //D
					kD = false;
				break;
                case 0x51: //Q
					kQ = 0.0;
				break; 
                case 0x45: //E
					kE = 0.0;
				break;
            } 
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
        return 0;
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
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}
