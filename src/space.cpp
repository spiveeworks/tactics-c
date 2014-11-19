
//#include "..\spivee"
#include <vector>
#include "point.cpp"

/*
class worldpoint : public point {
	
	
};
*/


POINT mkPOINT (long x, long y) {
	POINT retval; 
	retval.x = x; 
	retval.y = y; 
	return retval;
}

#define AXIS_LENGTH 100

struct camera {
	SGeo::point centre;
	POINT graphic;
	SGeo::vector axis;
public:
	POINT world_to_screen (SGeo::point world) {
		SGeo::vector loc = (world - centre)/axis;
		POINT retval (graphic);
		retval.x += loc.x*AXIS_LENGTH;
		retval.y -= loc.y*AXIS_LENGTH;
		return retval;
	}
	SGeo::point screen_to_world (POINT screen) {
		SGeo::vector pos (screen.x - graphic.x, graphic.y - screen.y);
		pos /= AXIS_LENGTH;
		return centre + (pos.x * axis + pos.y * axis.left ());
	}

	camera (SGeo::point cen = SGeo::point (0,0), SGeo::vector axi = SGeo::vector (1.0, 0.0), POINT gra = mkPOINT (512,384))
	{
		centre = cen;  
		axis = axi; 
		graphic = gra;
	}
	camera (SGeo::point cen, double angle, double scale = 1.0, POINT gra = mkPOINT (512,384))
	{
		centre = cen; 
		axis = SGeo::rad_to_coord (angle, scale); 
		graphic = gra; 
	}
	
	void rotate (double angle) {axis = outer (axis, SGeo::rad_to_coord (-angle));}
	void angle (double angle) {axis = SGeo::rad_to_coord (angle, axis.magnitude ());}
	
	void rescale (float factor) {axis *= factor;}
	void scale (double scale) {axis *= (scale / axis.magnitude ());}
	
	void resolve (RECT screen) {
		graphic = mkPOINT (
			(screen.left + screen.right) / 2, 
			(screen.top + screen.bottom) / 2
		);
	}
	
	void pan (SGeo::vector translate) {centre += translate;}
	
	
}MainCam;


#undef AXIS_LENGTH


struct color {
	unsigned char Rval, Gval, Bval;
	DWORD getRef () {return RGB (Rval, Gval, Bval);}
	color (unsigned char Red, unsigned char Green, unsigned char Blue) {Rval = Red; Gval = Green; Bval = Blue;}
	color (DWORD CRef = 0) {Rval = GetRValue (CRef); Gval = GetGValue (CRef); Bval = GetBValue (CRef);}
};

struct screenpoly {
	std::vector<POINT> vertices;
	color pen, brush;
	bool print (HDC dc) {
		//change brush and pen using mechanism to change back
		HGDIOBJ 
			savp = SelectObject(dc, GetStockObject (DC_PEN)),
			savb = SelectObject(dc, GetStockObject (DC_BRUSH));
		DWORD
			colp = SetDCPenColor(dc, pen.getRef()),
			colb = SetDCBrushColor(dc, brush.getRef());
		bool retval = Polygon (dc, &vertices[0], vertices.size ());
		SelectObject(dc, savp),
		SelectObject(dc, savb);
		SetDCPenColor(dc, colp),
		SetDCBrushColor(dc, colb);
		return retval;
	}
//tors
	screenpoly (POINT* vert, unsigned num, color penc = color(0), color brushc = color(0x00FFFFFF)) {vertices = std::vector<POINT> (vert, vert + num); pen = penc; brush = brushc;}
	
};

struct polygon {
	std::vector<SGeo::point> vertices;
	color pen, brush;
	screenpoly prerender (camera& ref) {
		POINT* points = new POINT [vertices.size()];
		for (unsigned i=0; i < vertices.size(); i++)
			points[i] = ref.world_to_screen (vertices [i]);
		screenpoly retval (points, vertices.size(), pen, brush);
		delete[] points;
		return retval;
	}
//tors
	polygon (SGeo::point* vert, unsigned num, color penc = color(0), color brushc = color(0x00FFFFFF)) {vertices = std::vector<SGeo::point> (vert, vert + num); pen = penc; brush = brushc;}
};








/*
struct screenpoly {
	int Vnum;
	int Maxnum;
	POINT* vertices;
	~screenpoly () {if (vertices != 0) delete[] vertices;}
	bool print (HDC out) {return Polygon (out, vertices, Vnum);}
	int addpoint (POINT toad) {
		//if (Vnum == Maxnum)
			//extendmax (1);
		vertices [Vnum] = toad;
		return ++Vnum;
	}
	int extendmax (int toext) {
		POINT* next;
		next = new POINT [Maxnum + toext];
		//copy (Vnum, vertices, next);
		if (Maxnum != 0)
			delete[] vertices;
		vertices = next;
		return (Maxnum += toext);
	}
	screenpoly () {Vnum = 0; Maxnum = 0; vertices = 0;}
	POINT operator[] (int X) {return vertices[X];}
	void operator= (screenpoly comp) {
		if (vertices != 0 && Maxnum != 0)
			delete[] vertices;
		Vnum = comp.Vnum;
		Maxnum = comp.Maxnum;
		vertices = new POINT [Maxnum];
		copy (Vnum, comp.vertices, vertices);
	}
	int debugadd (POINT toad, POINT& debug) {debug = toad; return addpoint (toad);}
};
*/

