#include <vector>
    //Standard variable-length-array objects

#include "Vect.cpp"
    //My cartesian geometry stuff, for vectors and some trigonometry

    
    namespace Tact {

struct point: SCG::point {
    
    point (double x = 0.0, double y = 0.0): SCG::point(x, y) {}
    point (SCG::vector location): SCG::point(location) {}
    point (SCG::point base): SCG::point(base) {}
};

union color {
    long CRef; // windows.h defines DWORD which is much better for this purpose, but I'll leave it as long, so as to make Tactics independent of its windows interface.
    struct {unsigned char R, G, B;} part;
    unsigned char el[3];
};

class basic_block {
public:
    std::vector<point> vertices;
    color fill;
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
