#include "point.cpp"
#include <iostream>



int main () {
	SGeo::vector test = SGeo::rad_to_coord (1.0, 5.0);
	SGeo::vector axis = SGeo::rad_to_coord (1.0, 1.0);
	test = test / axis;
	std::cout << test.x << ", " << test.y;
	
	return 0;
}