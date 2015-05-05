
camera playercam;
RECT gameClient;

struct dcStruct {
private:
	bool colstate;
public:
    HDC dc;
    
    HGDIOBJ penobj, bruobj;
    DWORD pencol, brucol;
	bool prepCol (DWORD pen, DWORD brush) {
		bool out = !colstate;
		if (out) {
			penobj = SelectObject(dc, GetStockObject (DC_PEN)); //choose customisable object
			bruobj = SelectObject(dc, GetStockObject (DC_BRUSH)); //same for border
			pencol = SetDCPenColor(dc, pen); //set chosen color
			brucol = SetDCBrushColor(dc, brush);
			colstate = true;
		}
		else
		{
			SetDCPenColor(dc, pen); //set chosen color
			SetDCBrushColor(dc, brush);
		}
		return out;
	}
	void popCol ();
    
    
};



struct screenpoly {
    std::vector<POINT> vertices;
    DWORD pen, brush;
    bool print (dcStruct dat) {
		dat.prepCol(pen, brush); //Use smart struct to handle colors efficiently
        return Polygon (dat.dc, &vertices[0], vertices.size ()); //get API to handle the rest, and pass the result back.
    }
//tors
    screenpoly (
        POINT* vert, unsigned num, 
        DWORD penc = 0, DWORD brushc = 0x00FFFFFF) 
    {
        vertices = std::vector<POINT> (vert, vert + num); 
        pen = penc; brush = brushc;
    }
    screenpoly (camera& cam, Tact::basic_block& base, DWORD penc = 0x00FFFFFF, DWORD brushc = 0) {
        copair* vert = batchdraw(cam, base.vertices);
        POINT* begin = (POINT*)vert;
        POINT* end = begin + base.vertices.size();
        vertices = std::vector<POINT> (begin, end);
        delete[] vert;
        pen = penc; brush = brushc;
    }
};


screenpoly RECT2pol (RECT Bs, DWORD pen, DWORD brush) {
	POINT* vert = new POINT [4];
	vert[0].y = vert[1].y = Bs.top;
	vert[3].y = vert[2].y = Bs.bottom;
	vert[0].x = vert[3].x = Bs.left;
	vert[1].x = vert[2].x = Bs.right;
	return screenpoly(vert, 4, pen, brush);
}

screenpoly RECT2pol (long* Bs, DWORD pen, DWORD brush) {
	POINT* vert = new POINT [4];
	vert[0].y = vert[1].y = Bs[0];
	vert[3].y = vert[2].y = Bs[1];
	vert[0].x = vert[3].x = Bs[2];
	vert[1].x = vert[2].x = Bs[3];
	return screenpoly(vert, 4, pen, brush);
}




void init () {
    srand(GetCurrentTime());
    gameClient.left = 50;
    gameClient.top = 50;
    
    gameClient.right = 550;
    gameClient.bottom = 550;
    
    playercam.rec = *(bounds*)&gameClient;
    
    

}


int v = 0;

void render (dcStruct dat, RECT& region) {
    RECT eq;
    eq.top = eq.left = 0;
    eq.right = region.right - region.left;
    eq.bottom = region.bottom - region.top;
    bounds oldrec = playercam.rec;
    playercam.rec = *(bounds*)& eq;
    
    //initialise buffer
    dcStruct buffer;
    buffer.dc = CreateCompatibleDC(dat.dc);
    HBITMAP hbmBuffer = CreateCompatibleBitmap(dat.dc, eq.right, eq.bottom);
    HBITMAP hbmOldBuffer = (HBITMAP)SelectObject(buffer.dc, hbmBuffer);
    
    
    
    //Empty the entire screen
    FillRect(buffer.dc, &eq, (HBRUSH)GetStockObject(BLACK_BRUSH));
    
    Tact::basic_block pol;
    
    pol.vertices.push_back(Tact::point(-20.0, -20.0));
    pol.vertices.push_back(Tact::point(+20.0, -20.0));
    pol.vertices.push_back(Tact::point(+20.0, +20.0));
    pol.vertices.push_back(Tact::point(-20.0, +20.0));
    pol.fill.CRef = 0x00FFFFFF;
    screenpoly out ((POINT*)batchdraw(playercam, pol.vertices), pol.vertices.size(), 0x00FFFFFF, pol.fill.CRef);
    out.print(buffer);
    //Rectangle(dat.dc, 105, 115, 100, 110);
    
    
    
    
    std::stringstream s;
    s << v++ << ": " << (long)playercam.focus.x() << ", " << (long)playercam.focus.y();
    std::string sstr = s.str();
    DrawText(buffer.dc, TEXT(sstr.c_str()),sstr.length(), &eq, DT_TOP|DT_LEFT);

    
    //flush buffer
    BitBlt(dat.dc, region.left, region.top, region.right, region.bottom, buffer.dc, 0, 0, SRCCOPY);

	//Reset and delete buffer DC and then buffer bitmap
    SelectObject(buffer.dc, hbmOldBuffer);
    DeleteDC(buffer.dc);
    DeleteObject(hbmBuffer);
    
    playercam.rec = oldrec;
}






    
/*    Tact::point p (25.0, 0.0);
    copair o = cam.draw(p);
    std::stringstream s;
    s << o.x << ", " << o.y;
    std::string sstr = s.str();
    DrawText(dat.dc, TEXT(sstr.c_str()),sstr.length(), &rcClient, DT_TOP|DT_LEFT);
*/
