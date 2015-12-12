#include <vector>
    //Standard variable-length-array objects

#include "Vect.cpp"
    //My cartesian geometry stuff, for vectors and some trigonometry
#include "Line.cpp"
    //Line object that was made specifically for projectiles in this program, but it fits better as an SCG extension

//Note the difference between std::vector<T> and SCG::vector (sorry about that)

    namespace Tact {

typedef SCG::vector vector;
typedef SCG::point point;

union color 
{
    long CRef; // windows.h defines DWORD which is much better for this purpose, but I'll leave it as long, so as to make Tactics independent of its windows interface.
    struct {unsigned char R, G, B;} part;
    unsigned char el[3];
};

class Block 
{
public:
    std::vector<point> vertices;
    color fill;
	point& operator[] (int i) 
        {return vertices[i % vertices.size()];}
	Base (std::vector<point> vertices_c = std::vector<point>(), color fill_c = color()) 
        {vertices = vertices_c; fill = fill_c;}
    SCG::line line (int i)
        {return SCG::produce(operator[](i), operator[](i + 1));}
};


class Projectile 
{
private:
	SCG::line xy; //xy.p is current location, xy.dir is velocity vector
	double sz, vz;
public:
	void Displace(double dt, double g) 
    {
		sz += dt * (vz - g * dt / 2); //projectile motion woo
		vz -= dt * g;
		
		xy.p += dt * xy.dir;
	}
    double PredictCollision (SCG::line l)
        {return xy.coefficient(l);}
    //gives the dt value associated with this projectile colliding with line L
    double PredictCollision (Agent c)
        {return xy.coefficient(c.pos, c.radius * c.radius).first;}
    bool TestCollision (Agent c)
        {return (xy - c.pos).square <= c.radius * c.radius;}
};

Projectile PredictProjectile (Projectile b, double dt, double g)
{
    b.displace(dt, g);
    return b;
}

class Agent 
{
public:
	point pos;
	double radius;
};





    } // end namespace

/*
 * NOTE
 *
 * I need to have this file be called "Tact.cpp" 
 * and then move the Tact namespace to something called 
 * env or engine or objects or something, 
 * so that includes are together at the start of this file, 
 * instead of the more messy putting the include after the namespace.
 *
 */

#include "rendel.h"
    //Details that make interfacing easy, without actually implementing an interface just yet.
