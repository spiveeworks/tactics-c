
#include "handle.cpp"

/* 
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
 */


struct screenpoly {
    std::vector<POINT> vertices;
    DWORD pen, brush;
    bool print (paintable& out) {return out.drawPolygon (&vertices[0], vertices.size (), pen, brush);}
//tors
    screenpoly (
        POINT* vert, unsigned num, 
        DWORD penc = 0, DWORD brushc = 0x00FFFFFF) 
    {
        vertices = std::vector<POINT> (vert, vert + num); 
        pen = penc; brush = brushc;
    }
    screenpoly (camera& cam, Tact::block_base& base, DWORD penc = 0x00FFFFFF, DWORD brushc = 0) {
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




RECT Bound2Rect(bounds base) {
	RECT out;
	out.left	= base.dir.west;
	out.top		= base.dir.north;
	out.right	= base.dir.east;
	out.bottom	= base.dir.south;
	return out;
}

void render (HDC dat, RECT ClientRect, camera &playercam) {//I need to rip this abomination to shreds some time
    RECT eq = Bound2Rect(playercam.rec);
    DDBHandler buffer;
	RECT BufferRect;
    
    //initialise buffer
	BufferRect.left = BufferRect.top = 0;
	BufferRect.right = ClientRect.right - ClientRect.left;
	BufferRect.top = ClientRect.bottom - ClientRect.top;
	
	buffer.open(dat, BufferRect.right, BufferRect.bottom);
	
    //Empty buffer bitmap
    FillRect(buffer.get(), &BufferRect, (HBRUSH)GetStockObject(BLACK_BRUSH));
    
	//ad-hoc poly construction
	Tact::block_base pol;
    
    pol.vertices.push_back(Tact::point(-20.0, -20.0));
    pol.vertices.push_back(Tact::point(+20.0, -20.0));
    pol.vertices.push_back(Tact::point(+30.0, 0.0));
    pol.vertices.push_back(Tact::point(+20.0, +20.0));
    pol.vertices.push_back(Tact::point(-20.0, +20.0));
	
	//draw it
    pol.fill.CRef = 0x000088FF;
    screenpoly out ((POINT*)batchdraw(playercam, pol.vertices), pol.vertices.size(), 0x00FFFFFF, pol.fill.CRef);
    out.print(buffer);
    
    
    //flush buffer
    BitBlt(dat, eq.left, eq.top, eq.right - eq.left, eq.bottom - eq.top, buffer.get(), eq.left, eq.top, SRCCOPY);
    //BitBlt(dat, 50, 50, 500, 500, buffer.get(), 50, 50, SRCCOPY);

	//Reset and delete buffer DC and then buffer bitmap
    buffer.close();
}






    
/*    Tact::point p (25.0, 0.0);
    copair o = cam.draw(p);
    std::stringstream s;
    s << o.x << ", " << o.y;
    std::string sstr = s.str();
    DrawText(dat.dc, TEXT(sstr.c_str()),sstr.length(), &rcClient, DT_TOP|DT_LEFT);
*/