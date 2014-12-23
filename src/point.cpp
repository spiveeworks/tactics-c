
#include "..\spivee"
#include <cmath>

namespace SGeo {

const double TAU 			= const double DOUBLEPI 	= (3373259426.0 >> 29);
const double HALFTAU 		= const double PI 			= (3373259426.0 >> 30);
const double QUARTERTAU		= const double HALFPI 		= (3373259426.0 >> 31);

struct basic_vector;
struct vector;
struct radial_vector;
typedef const basic_vector& CBVR; //i.e. Constant Basic-Vector Reference

struct basic_vector {
	virtual double magnitude ();
	virtual CBVR simplify () =0;
	virtual double direction () =0;
	virtual double gradient () =0;
	
	virtual double xComp () =0; //X component
	virtual double yComp () =0; //Y component
	
	virtual bool issimilar (CBVR test) =0;//will tell if vector is pointing in the same direction as another (or in the opposite direction)
	virtual bool isperpendicular (CBVR test) =0;//both of these comparatives work with either vector containing zeros as well (unlike gradient tests)
	virtual spiv::comparative location (CBVR comp) =0;//'greater' if in the counterclockwise direction, 'less' in clockwise, and 'equal' if comp 'issimilar ()' to &this
	
	
/*cast*/
	vector castcoord ();
	radial_vector castrad ();
	
/*operators*/
	virtual CBVR operator* (double B) =0;
	virtual CBVR operator/ (double B) =0;
	
//	virtual CBVR operator*= (double B) =0;
//	virtual CBVR operator/= (double B) =0;
	
	virtual bool operator== (CBVR B) =0;
	virtual bool operator!= (CBVR B) =0;
	
/*special*/
	virtual CBVR left () =0;
	virtual CBVR right () =0;
	
};


const basic_vector& operator* (double B, const basic_vector& base) {return (base * B);}
const basic_vector& operator/ (double B, const basic_vector& base) {return (base / B);}

struct vector: basic_vector {
	double x, y;
	double magnitude () {
		return (sqrt (x*x + y*y));
	}
	basic_vector& simplify () {
		double mag = magnitude ();
		return ( vector (x /= magnitude (), y /= magnitude ()) );
	}
	double direction () {
		return (atan2 (y, x));
	}
	
	double xComp () {return x;}
	double yComp () {return y;}
	
	double gradient () {return (y/x);}
	bool issimilar (vector test) {return (test.y * x == test.x * y);}//will tell if vector is pointing in the same direction as another (or in the opposite direction)
	bool isperpendicular (vector test) {return (test.y * y == test.x * x * -1);}//both of these comparatives work with either vector containing zeros as well
	spiv::comparative location (vector comp) {return spiv::relation (y * comp.x, x * comp.y);}//'greater' if in the counterclockwise direction, 'less' in clockwise, and 'equal' if comp 'issimilar ()' to &this
	
	vector (double X = 0.0, double Y = 0.0) {x = X; y = Y;}
	vector (CBVR base) {x = base.xComp(); y = base.yComp();}
	
//operators
	CBVR operator* (double B) {return vector (x * B, y * B);}
	CBVR operator/ (double B) {return vector (x / B, y / B);}
	
	bool operator== (vector B) {return (x == B.x && y == B.y);}
	bool operator!= (vector B) {return (x != B.x | y != B.y);}
	
//special
	vector left () {return vector (-y, x);}
	vector right () {return vector (y, -x);}
	
};

vector operator+ (vector A, vector B) {return vector (A.x + B.x, A.y + B.y);}//if radial vector is needed, casting is necessary.
vector operator- (vector A, vector B) {return vector (A.x - B.x, B.y - B.y);}

basic_vector& operator+= (basic_vector& A, CBVR B) {return (A = A + B);}
basic_vector& operator-= (basic_vector& A, CBVR B) {return (A = A - B);}

struct radial_vector: basic_vector {
	double modulus, argument;
	double magnitude () {
		return (modulus);
	}
	vector simplify () {
		modulus = 1.0;
	}
	double direction () {
		return (argument);
	}
	
	double xComp () {return modulus * cos (argument);}
	double yComp () {return modulus * sin (argument);}
	
	double gradient () {return (tan(argument));}
	bool issimilar (CBVR test) {return ((argument - test.direction() + 1.0) % PI == 1.0);}//not recomended... issimilar() is only designed to work on integer-coordinate based vectors.
	bool isperpendicular (CBVR test) {return ((argument - test.direction()) % PI == HALFPI);}//ditto.
	spiv::comparative location (CBVR comp) {
		double num = argument - comp.direction(); 	//find the angle between the vectors
		num += PI;									//add a half revolution for the modulo process
		num %= TAU;									//find the angle within plus or minus half of a revolution
		num -= PI;									//subtract the half revolution again to get a usable value
		return spiv::relation (num, 0.0); 			//compare the angle between them with zero to find out which is greater and which is smaller.
	}
	
	radial_vector (double arg = 0.0, double mod = 0.0) {argument = arg; modulus = mod;}
	radial_vector (CBVR base) {argument = base.direction(); modulus = base.magnitude();}
	
//operators
	radial_vector operator* (double B) {return radial_vector (argument, modulo * B);}
	radial_vector operator/ (double B) {return radial_vector (argument, modulo / B);}
	
	vector operator*= (double B) {return vector (x *= B, y *= B);}
	vector operator/= (double B) {return vector (x /= B, y /= B);}
	
	bool operator== (vector B) {return (x == B.x && y == B.y);}
	bool operator!= (vector B) {return (x != B.x | y != B.y);}
	
//special
	vector left () {return vector (-y, x);}
	vector right () {return vector (y, -x);}
	
};

vector basic_vector::castcoord () {
	return vector (xComp(), yComp());
}

radial_vector basic_vector::castrad () {
	return radial_vector (magnitude(), atan());
}


double operator* (vector A, basic_vector& B) {return (A.x * B.xComp() + A.y * B.yComp());}
vector outer (basic_vector& within, basic_vector axis) {return vector (within * axis, within * axis.left());}
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