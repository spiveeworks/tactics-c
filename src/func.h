
void RenderScreen(HDC hdc, RECT* prc);

bool printpoint (HDC device, int X, int Y, POINT toprint, char* name);
bool paintpoint (HDC device, color pencol, POINT todraw);
bool printnum (HDC device, int X, int Y, int toprint, char* name);

void randomizecolor (color& toset);

///// BEGIN /////

void RenderScreen(HDC hdc, RECT* prc)
{

	HDC hdcBuffer = CreateCompatibleDC(hdc);
	HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, prc->right, prc->bottom);
	HBITMAP hbmOldBuffer = (HBITMAP)SelectObject(hdcBuffer, hbmBuffer);

	FillRect(hdcBuffer, prc, (HBRUSH)GetStockObject(WHITE_BRUSH));


/*
	for (unsigned i = 0; i < blocks.size(); i++)
		blocks[i] -> prerender (MainCam).print (hdcBuffer);

	current.prerender (MainCam).print (hdcBuffer);
*/
	for (unsigned i = 0; i < plots.size (); i++)
		paintpoint (hdcBuffer, color (0), MainCam.world_to_screen (plots[i]) );
	BitBlt(hdc, 0, 0, prc->right, prc->bottom, hdcBuffer, 0, 0, SRCCOPY);

	SelectObject(hdcBuffer, hbmOldBuffer);
	DeleteDC(hdcBuffer);
	DeleteObject(hbmBuffer);
}




bool printpoint (HDC device, int X, int Y, POINT toprint, char* name) {
	stringstream concatenate;
	concatenate << name << ": " << toprint.x << ", " << toprint.y;
	string display = concatenate.str ();
	return TextOut(device, X, Y, display.c_str (), display.length () );
}

bool paintpoint (HDC device, color pencol, POINT todraw) {
	
	MoveToEx(device, todraw.x - 5, todraw.y - 5, NULL);
	LineTo(device, todraw.x + 5, todraw.y + 5);
	MoveToEx(device, todraw.x + 5, todraw.y - 5, NULL);
	LineTo(device, todraw.x - 5, todraw.y + 5);
}

bool printnum (HDC device, int X, int Y, int toprint, char* name) {
	stringstream concatenate;
	concatenate << name << ": " << toprint;
	string display = concatenate.str ();
	return TextOut(device, X, Y, display.c_str (), display.length () );
}


void randomizecolor (color& toset) {
	unsigned char *fill, *grad, *empty, *left, *right;
	switch (rand() % 3) {
		case 0:
			fill = &toset.Rval;
			left = &toset.Gval;
			right = &toset.Bval;
		break;
		case 1:
			fill = &toset.Gval;
			left = &toset.Rval;
			right = &toset.Bval;
		break;
		case 2:
			fill = &toset.Bval;
			left = &toset.Rval;
			right = &toset.Gval;
		break;
	}
	if (rand() % 2 == 1) {
		grad = left;
		empty = right;
	}
	else
	{
		empty = left;
		grad = right;
	}
	*fill = 255;
	*grad = rand() % 256;
	*empty = 0;
}