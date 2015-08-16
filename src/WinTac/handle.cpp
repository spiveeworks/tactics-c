
class paintable {
public:
	virtual bool drawPolygon (POINT *arr, unsigned size, DWORD pen = 0x00000000, DWORD brush = 0x00FFFFFF) = 0;
	//bool drawRect (RECT todraw, DWORD pen = 0x00000000, DWORD brush = 0x00FFFFFF) = 0;
};

class DCHandler: public paintable {
protected:
	HDC handle;
	bool hasColors;
	HGDIOBJ penobj_p, brushobj_p;
    DWORD pen_p, brush_p;
	
	
public:
	DCHandler() {handle = NULL; hasColors = false;}
	
	bool push_colors(DWORD pen, DWORD brush);//doesn't actually form a stack.
	bool pop_colors();
	bool drawPolygon (POINT *arr, unsigned size, DWORD pen = 0x00000000, DWORD brush = 0x00FFFFFF);
	
	virtual bool close() = 0;
	
	HDC get() const {return handle;}
	
};

class screenHandler: public DCHandler {
protected:
	HWND window_handle;
public:
	virtual HDC open(HWND window) {return handle = GetDC(window);}
	//screenHandler(HWND window_handle_c): DCHandler(scrInit(window_handle_c)) {window_handle = window_handle_c;}
	//~screenHandler() {ReleaseDC(window_handle, handle);} //do default destructors call base destructors
	HWND get_window() const {return window_handle;}
	virtual bool close() {ReleaseDC(window_handle, handle);}
};

class screenPaintHandler: public screenHandler {
protected:
public:
	PAINTSTRUCT ps;
	
	virtual HDC open(HWND window) {window_handle = window; return handle = BeginPaint(window_handle, &ps);}
	virtual bool close() {EndPaint(window_handle, &ps);}
};

class DDBHandler: public DCHandler {
protected:
	HBITMAP image_handle, image_handle_mono;
public:
	virtual HDC open(HDC base_dc, int width, int height) {
		handle = CreateCompatibleDC(base_dc);
		image_handle = CreateCompatibleBitmap(base_dc, width, height);
		image_handle_mono = (HBITMAP)SelectObject(handle, image_handle);
		return handle;
	}
	bool close() {
		SelectObject(handle, image_handle_mono);
		DeleteDC(handle);
		DeleteObject(image_handle);
	}
};

bool DCHandler::push_colors(DWORD pen, DWORD brush) {
	bool out = false;
	if (hasColors) {
		SetDCPenColor	(handle, pen); //set chosen color
		SetDCBrushColor	(handle, brush);
	}
	else
	{
		penobj_p = 		SelectObject	(handle, GetStockObject (DC_PEN)); //choose customisable object
		brushobj_p = 	SelectObject	(handle, GetStockObject (DC_BRUSH)); //same for border
		pen_p = 		SetDCPenColor	(handle, pen); //set chosen color
		brush_p = 		SetDCBrushColor	(handle, brush);
		out = hasColors = true;
	}
	return out;
}

bool DCHandler::pop_colors() {
	if (!hasColors)
		return false;
	SetDCPenColor	(handle, pen_p);
	SetDCBrushColor	(handle, brush_p);
	SelectObject	(handle, penobj_p);
	SelectObject	(handle, brushobj_p);
	return true;
}

bool inline DCHandler::drawPolygon (POINT *arr, unsigned size, DWORD pen, DWORD brush) {
	push_colors(pen, brush);
	return Polygon(handle, arr, size);
}
