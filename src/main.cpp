#include <windows.h>
#include "head.h"
#include <string>
#include <sstream>
#include "space.cpp"

using namespace std;

//need to make systems for camera to screen mutual updates, and for camera to calculate screenpoly vector for prerender buffering to be efficient!
const char g_szClassName[] = "myWindowClass";
const int ID_TIMER = 1;

#define WM_RECURSE (WM_USER + 1)


polygon current (0, 0);
vector<polygon*> blocks;

bool kW, kA, kS, kD;
double kQ, kE;

vector<SGeo::point> plots;

#include "func.h"

//screenpoly* current = new screenpoly;
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{	
		case WM_LBUTTONUP:
		{
			POINT endP;
			GetCursorPos (&endP);
			ScreenToClient (hwnd, &endP);
			/*
			current.vertices.push_back (MainCam.screen_to_world (endP));
			if (current.vertices.size() == 5)
			{
				blocks.push_back (new polygon (current));
				current.vertices.resize (0);
				randomizecolor (current.brush);
			}/**/
			plots.push_back (MainCam.screen_to_world (endP));
			break;
		}
		case WM_CREATE:
		{
			current.vertices.reserve (10);
			srand (GetCurrentTime());
			//PostMessage (hwnd, WM_RECURSE, 0, GetCurrentTime ());
			//MainCam.angle (0.785398163397448309615);
			//BITMAP bm;

			/*
			UINT ret;
			ret = SetTimer(hwnd, ID_TIMER, 50, NULL);
			if(ret == 0)
				MessageBox(hwnd, "Could not SetTimer()!", "Error", MB_OK | MB_ICONEXCLAMATION);/**/
		}
		break;
		case WM_CLOSE:
			DestroyWindow(hwnd);
		break;
		case WM_PAINT:
		{
			RECT rcClient;
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);

			GetClientRect(hwnd, &rcClient);
			RenderScreen (hdc, &rcClient);

			EndPaint(hwnd, &ps);
		}
		break;
		case WM_MOUSEMOVE:
		{	
				if (current.vertices.size() != 0)
				{
			RECT rcClient;
			HDC hdc = GetDC(hwnd);
			
			GetClientRect(hwnd, &rcClient);
/*
			POINT coord;
			GetCursorPos (&coord);
			ScreenToClient (hwnd, &coord);
			current.vertices.push_back (MainCam.screen_to_world (coord));
*/
			
			RenderScreen (hdc, &rcClient);
			
//			current.vertices.pop_back ();
			ReleaseDC(hwnd, hdc);
				}
		}
		break;
		/*case WM_KEYDOWN: 
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
				case 0x20:
					PostMessage (hwnd, WM_RECURSE, 0, GetCurrentTime ());
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
		break;*/
		/*
		case WM_KEYDOWN: 
			switch (wParam) 
                case 0x51: //Q
					MainCam.angle (0.785398163397448309615);
				break; 
		break;
		case WM_KEYUP: 
			switch (wParam) 
            { 
                case 0x51: //Q
					MainCam.angle (0.0);
				break; 
            } 
		break;
		*/
		/*
		case WM_RECURSE:
		{
			SGeo::vector move (0.0,0.0);
			if (kW) move.y++;
			if (kS) move.y--;
			if (kA) move.x++;
			if (kD) move.x--;
			move.simplify();
			move *= 0.001;
			MainCam.pan (move);
			MainCam.rotate (kQ + kE);
			/*
			RECT rcClient;
			HDC hdc = GetDC(hwnd);
			
			GetClientRect(hwnd, &rcClient);

			POINT coord;
			GetCursorPos (&coord);
			ScreenToClient (hwnd, &coord);
			current.vertices.push_back (MainCam.screen_to_world (coord));

			
			RenderScreen (hdc, &rcClient);
			
			current.vertices.pop_back ();
			ReleaseDC(hwnd, hdc);
			
			//PostMessage (hwnd, WM_RECURSE, 0, GetCurrentTime ());
			
		}
		break;
		*/
		case WM_DESTROY:
			//KillTimer(hwnd, ID_TIMER);

			PostQuitMessage(0);
		break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

#include "windows.h"

