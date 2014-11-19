
#include "..\spivee"
#include <cmath>

namespace SGeo {
struct vector {
	double x, y;
	double magnitude () {
		return (sqrt (x*x + y*y));
	}
	vector simplify () {
		double mag = magnitude ();
		return ( vector (x /= magnitude (), y /= magnitude ()) );
	}
	double atan () {
		return (atan2 (y, x));
	}
	double gradient () {return (y/x);}
	bool issimilar (vector test) {return (test.y * x == test.x * y);}//will tell if vector is pointing in the same direction as another (or in the opposite direction)
	bool isperpendicular (vector test) {return (test.y * y == test.x * x * -1);}//both of these comparatives work with either vector containing zeros as well
	vector (double X = 0.0, double Y = 0.0) {x = X; y = Y;}
	spiv::comparative location (vector comp) {return spiv::relation (y * comp.x, x * comp.y);}//'greater' if in the counterclockwise direction, 'less' in clockwise, and 'equal' if comp 'issimilar ()' to &this
//operators
	vector operator* (double B) {return vector (x * B, y * B);}
	vector operator/ (double B) {return vector (x / B, y / B);}
	vector operator+ (vector B) {return vector (x + B.x, y + B.y);}
	vector operator- (vector B) {return vector (x - B.x, y - B.y);}
	
	vector operator+= (vector B) {return vector (x += B.x, y += B.y);}
	vector operator-= (vector B) {return vector (x -= B.x, y -= B.y);}
	vector operator*= (double B) {return vector (x *= B, y *= B);}
	vector operator/= (double B) {return vector (x /= B, y /= B);}
	
	bool operator== (vector B) {return (x == B.x && y == B.y);}
	bool operator!= (vector B) {return (x != B.x | y != B.y);}
	
//special
	vector left () {return vector (-y, x);}
	vector right () {return vector (y, -x);}
	
};

vector operator* (double B, vector base) {return (base * B);}
vector operator/ (double B, vector base) {return (base / B);}

double operator* (vector A, vector B) {return (A.x * B.x + A.y * B.y);}
vector outer (vector within, vector axis) {return vector (within * axis, within * axis.left());}
vector operator/ (vector within, vector axis) {return (outer (within, axis)/(axis*axis));}
//If you rotate both vectors such that "axis" lies along the x axis, then the result of this operation is the new vector "within" divided by the magnitude of axis;

double atan (vector Vec) {
	return ( atan2 (Vec.y, Vec.x) );
}

vector rad_to_coord (double angle, double magnitude = 1.0) {
	return (magnitude * vector (cos (angle), sin (angle)));
}

class point {
	vector fromorigin;
	
public:
	
	double x () {return fromorigin.x;}
	double y () {return fromorigin.y;}
	vector coords () {return fromorigin;}
	
	//function members
	
	double gradient (point p) {return ( (p.fromorigin - fromorigin).gradient () );}
	point midpoint (point p) {return ( point ( (p.fromorigin.x - fromorigin.x)/2, (p.fromorigin.y - fromorigin.y)/2 ) );}
	double distance (point p) {return ( (p.fromorigin - fromorigin).magnitude () );}
	
	//structifiers
	
	point (double x = 0.0, double y = 0.0) {
		fromorigin.x = x;
		fromorigin.y = y;
	}
	point (vector location) {
		fromorigin = location;
	}
	
	//operators
	
	point operator+ (vector B) {return point (fromorigin + B);}
	point operator- (vector B) {return point (fromorigin - B);}
	vector operator- (point B) {return (fromorigin - B.fromorigin);}
	
	point operator+= (vector B) {return point (fromorigin += B);}
	point operator-= (vector B) {return point (fromorigin -= B);}
	vector operator-= (point B) {return (fromorigin -= B.fromorigin);}
	
	point displace (double U, double D) {return point (fromorigin += vector (U, D));}
	point counterplace (double U, double D) {return point (fromorigin -= vector (U, D));}
	//same as += and -= respectively, just with both coords as arguments
	
	bool operator== (point B) {return (fromorigin == B.fromorigin);}
	bool operator!= (point B) {return (fromorigin != B.fromorigin);}
	
};


}