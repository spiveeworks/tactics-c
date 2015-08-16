
#include <cmath>

namespace SCG { //begin namespace: Spivee's Cartesian Geometry.

double const TAU            = 6.2831853071795864769252867665591;
double const HALFTAU        = 3.1415926535897932384626433832795;
double const QUARTERTAU		= 1.5707963267948966192313216916397;

double const DOUBLEPI       = TAU;
double const PI             = HALFTAU;
double const HALFPI         = QUARTERTAU;

struct vector;




/* * * * * * * * * **/
/* Cartesian Vector */
/** * * * * * * * * */

struct vector {//standard 2-dimensional cartesian coordinated vector
    double x, y;
/*struct*/
    vector (double xc = 0.0, double yc = 0.0) {x = xc; y = yc;}
    vector (const vector& base) {x = base.x; y = base.y;}
    vector& operator= (const vector& base) {x = base.x; y = base.y; return *this;}
    
    
/*Basic Operations*/
    vector& operator*= (double B) {x *= B; y *= B; return *this;}
    vector& operator/= (double B) {x /= B; y /= B; return *this;}
    
    vector& operator+= (const vector& B) {x += B.x; y += B.y; return *this;}
    vector& operator-= (const vector& B) {x -= B.x; y -= B.y; return *this;}
    
/*Equality*/
    bool operator== (const vector& B) const {return (x == B.xComp()) && (y == B.yComp());}
    bool operator!= (const vector& B) const {return (x != B.xComp()) || (y != B.yComp());}
    
/*Properties, etc.*/
    double square () const {return (x*x + y*y);}
	double magnitude () const {return sqrt(square());}
    double direction () const {return atan2(y, x);}//not tested
    double gradient () const {return (y/x);}
    
    double xComp () const {return x;}
    double yComp () const {return y;}
    
    bool issimilar (const vector& test) const {return x*test.yComp() == y*test.xComp();}//will tell if vector is pointing in the same direction as another (or in the opposite direction)
    bool isperpendicular (const vector& test) const {return x*test.xComp() + y*test.yComp() == 0.0;}//both of these comparatives must work with either vector containing zeros as well (unlike gradient tests)
    signed location (const vector& comp) const {//+ve if in the counterclockwise direction, -ve in clockwise, and 0 if comp 'issimilar ()' to *this
		double x = x*comp.y - y*comp.x; 
		if (x==0.0) return 0; 
		if (x > 0.0) return +1; 
		return -1;
	}
/*Manipulation*/
	vector get_left () const {return vector(-y, x);}
	vector get_right () const {return vector(y, -x);}
	vector& push_left () {*this = get_left();}
	vector& push_right () {*this = get_right();}
    double operator* (const vector& B) const {return (x*B.xComp() + y*B.yComp());}
    vector rotated (vector rad) const {return vector( operator*(rad),operator*(rad.get_left()) );}
	//The transformation that rotates rad to the X-axis is applied to *this and returned.
	vector& rotate (vector rad) {return *this = rotated(rad);}
    vector rotated(double ang) const;
	vector& rotate(double ang);
    
};

vector operator* (vector A, double B) {A *= B; return A;}
vector operator/ (vector A, double B) {A /= B; return A;}
vector operator+ (vector A, vector const& B) {A += B; return A;}
vector operator- (vector A, vector const& B) {A -= B; return A;}
vector operator* (double B, const vector& base) {return base * B;} //for commutativity

vector mkVector (double argument) {return vector (cos(argument), sin(argument));}
vector mkVector (double argument, double modulo) {return mkVector(argument) * modulo;}
vector mkInvVector (double argument, double Inv_modulo = 1.0) {return mkVector(argument) / Inv_modulo;}

vector vector::rotated(double ang) const {return rotated(mkVector(-ang));}
vector& vector::rotate(double ang) {return *this = rotated(ang);}

/* * * * * * * * * */
/* Cartesian Point */
/* * * * * * * * * */

class point {
    vector fromorigin;
    
public:
    
    double& x () {return fromorigin.x;}
    double& y () {return fromorigin.y;}
    vector& coords () {return fromorigin;}
	
    double x () const {return fromorigin.x;}
    double y () const {return fromorigin.y;}
    vector coords () const {return fromorigin;}
    
    //function members
    
    double gradient (const point& p) const {return ( (p.fromorigin - fromorigin).gradient () );}
    point midpoint (const point& p) const {return point ( (fromorigin + p.fromorigin)/2 );}
    double distance (const point& p) const {return ( (p.fromorigin - fromorigin).magnitude () );}
    
    //structifiers
    
    point (double x = 0.0, double y = 0.0) {
        fromorigin = vector(x, y);
    }
    point (vector location) {
        fromorigin = location;
    }
    
    //operators
    
    point& operator+= (vector B) {fromorigin += B; return *this;}
    point& operator-= (vector B) {fromorigin -= B; return *this;}
    
    point operator+ (const vector& B) const {point ret (fromorigin); return (ret += B);}
    point operator- (const vector& B) const {point ret (fromorigin); return (ret -= B);}
    
    vector operator- (point B) const {return (fromorigin - B.fromorigin);}
    
    
    point translate (double U, double D) {return point (fromorigin += vector (U, D));}
    //same as +=, just with both coords as arguments
    
    bool operator== (point B) const {return (fromorigin == B.fromorigin);}
    bool operator!= (point B) const {return (fromorigin != B.fromorigin);}
    
};

double find_hor_int (point const& a, vector const& v, double const& ord = 0.0) {return (ord - a.y()) * v.x / v.y + a.x();} 
//this is just a rearranged point-gradient equation, which is the Cartesian equivalent to a vector line.

inline double find_hor_int (point const& p1, point const& p2, double const& ord = 0.0) {return find_hor_int(p1, (p2 - p1), ord);}

inline signed corner_orientation (point p1, point p2, point p3) {return (p2 - p1).location(p3 - p2);} //0 if collinear, 1 if CCW, -1 is CW (clockwise)

    } // end namespace

