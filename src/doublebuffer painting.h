void DrawBall(HDC hdc, RECT* prc)
{
	//initialise buffer
    HDC hdcBuffer = CreateCompatibleDC(hdc);
    HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, prc->right, prc->bottom);
    HBITMAP hbmOldBuffer = SelectObject(hdcBuffer, hbmBuffer);

	//prepare painting device and load first bitmap for painting process
    HDC hdcMem = CreateCompatibleDC(hdc);
    HBITMAP hbmOld = SelectObject(hdcMem, g_hbmMask);

	//Empty the entire screen
    FillRect(hdcBuffer, prc, GetStockObject(WHITE_BRUSH));

	//place mask of ball
    BitBlt(hdcBuffer, g_ballInfo.x, g_ballInfo.y, g_ballInfo.width, g_ballInfo.height, hdcMem, 0, 0, SRCAND);

	//load colour image bitmap and paint it straight away
    SelectObject(hdcMem, g_hbmBall);
    BitBlt(hdcBuffer, g_ballInfo.x, g_ballInfo.y, g_ballInfo.width, g_ballInfo.height, hdcMem, 0, 0, SRCPAINT);

	//flush buffer
    BitBlt(hdc, 0, 0, prc->right, prc->bottom, hdcBuffer, 0, 0, SRCCOPY);

	//Reset and delete painting DC
    SelectObject(hdcMem, hbmOld);
    DeleteDC(hdcMem);

	//Reset and delete buffer DC and then buffer bitmap
    SelectObject(hdcBuffer, hbmOldBuffer);
    DeleteDC(hdcBuffer);
    DeleteObject(hbmBuffer);
}