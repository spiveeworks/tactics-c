#include <vector>
    //Standard variable-length-array objects

#include "Vect.cpp"
    //My cartesian geometry stuff, for vectors and some trigonometry

//Note the difference between std::vector<T> and SCG::vector (sorry about that)

    namespace Tact {

typedef SCG::vector vector;
typedef SCG::point point;

class enclosed_builder {
private:
	std::vector<point> ps;
	SCG::vector left_arm, right_arm, left_shoulder;
	bool finished;
	mutable point worked;
	mutable SCG::vector from_left_m, from_right_m;
	
	int ind (int i) const {return i % ps.size();}
	void feed_in ();
	
public:
	enclosed_builder (point p1, point p2, point p3);
	enclosed_builder (enclosed_builder const& base, signed const& arm_index = -1);
	
	bool test (point p) const;
	bool add_point (point p);
	
	bool isopen() const {return !finished;}
	std::vector<point> const* get_shape() const {return &ps;}
	void abort() {finished = true;}
};


enclosed_builder::enclosed_builder (point p1, point p2, point p3) {
	ps.resize(0);
	ps.push_back(p1);
	ps.push_back(p2);
	ps.push_back(p3);
	
	left_arm = p1 - p2;
	right_arm = p3 - p2;
	left_shoulder = p3 - p1;
	
	finished = (left_arm.location(left_shoulder) == 0);
	
	worked = p3;
	from_right_m = right_arm;
	from_left_m = left_shoulder;
}

enclosed_builder::enclosed_builder (enclosed_builder const& base, signed const& arm_index) {
	ps.resize(base.ps.size());
	for (unsigned i = 0; i < base.ps.size(); i++)
		ps[i] = base.ps[(arm_index + i)%base.ps.size()];
	left_arm = ps[0] - ps[1];
	right_arm = ps[ind(-1)] - ps[ind(-2)];
	left_shoulder = ps[ind(-1)] - ps[0];
	
	finished = (left_arm.location(left_shoulder) == 0);
	
	worked = ps[ind(-1)];
	from_right_m = right_arm;
	from_left_m = left_shoulder;
}

void enclosed_builder::feed_in () {
	if (finished) return;
	if (worked != ps[ind(-1)])//if the last point equals the last checked point, this function need not change anything.
	{
		if (from_right_m.location(from_left_m) == 0)//if the point is in between the two end points, don't bother adding, just close the shape
			finished = true;
		else
		{
			if (right_arm.location(from_right_m) == 0)//if the point is collinear with the right arm, it will replace instead of appending
				ps.pop_back();//easy as
			
			ps.push_back(worked);         //
			right_arm = from_right_m;     //just moving data
			left_shoulder = from_left_m;  //
			
			if (left_arm.location(from_left_m) == 0) //if the point is collinear with the left arm, the shape must now be closed
			{
				ps[0] = ps[ind(-1)]; //maybe I should be using deques >.<
				ps.pop_back();
				finished = true;
			}
		}
	}
}

bool enclosed_builder::test (point p) const {
	if (finished) return false;
	SCG::vector from_left = p - ps[0];
	SCG::vector from_right = p - ps[ind(-1)];
	
	if (left_shoulder.location(from_left)*left_shoulder.location(left_arm) == -1)
		return false;
	if (right_arm.location(from_right)*right_arm.location(left_shoulder) == +1)
		return false;
	if (left_arm.location(from_left)*left_arm.location(left_shoulder) == -1)
		return false;
	
	//by now p is certainly within the shape, and can be stored for later use.
	worked = p;
	from_left_m = from_left;
	from_right_m = from_right;
	return true;
}

bool enclosed_builder::add_point (point p) {
	if (p != worked) //if p hasn't been checked recently
		if (!test(p)) //then check.
			return false; //should the check fail, just return false because it didn't work.
	feed_in(); //if it hasn't returned yet, use the feed_in function
	return true; //hence it worked
}


union color {
    long CRef; // windows.h defines DWORD which is much better for this purpose, but I'll leave it as long, so as to make Tactics independent of its windows interface.
    struct {unsigned char R, G, B;} part;
    unsigned char el[3];
};

class block_base {
public:
    std::vector<point> vertices;
    color fill;
	point& operator[] (int i) {return vertices[i % vertices.size()];}
	block_base(std::vector<point> vertices_c = std::vector<point>(), color fill_c = color()) {vertices = vertices_c; fill = fill_c;}
};

class block: private block_base {
public:
	block (enclosed_builder shape, color fill_c): block_base (*shape.get_shape(), fill_c) {}

};

class projectile {
private:
	vector sxy, vxy; //sexy
	double sz, vz;
public:
	void displace(double dt, double g) {
		sz += dt * (vz - g * dt / 2); //projectile motion woo
		vz -= dt * g;
		
		sxy += dt * vxy;
	}
};

class agent {
public:
	vector pos;
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
