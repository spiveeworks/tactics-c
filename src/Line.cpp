

namespace SCG { //begin namespace: Spivee's Cartesian Geometry.

struct line 
{
    point p;
    vector dir;
    
    //get coefficient required to find the intercept of these lines
    //this->operator()(this->coefficient(L)) will give the actual intercept
    double coefficient (line L);
    pair<double, double> coefficients (point centre, double rsqr); //note radius squared is asked for
    point operator() (double t)
        {return p + t * dir;}
    //get intercepts
    point operator* (line L)
        {return operator()(coefficient(L));}
};

//get coefficient required to find the intercept of these lines
//this->operator()(this->coefficient(L)) will give the actual intercept
double line::coefficient (line L)
{
    vector right = dir.get_right();
    return ((p - L.p) * right) / (L.dir * right); //algebra not included
    //I got this by solving two gradient-intercept equations, and then factorising the xs and ys into vectors
    //note that L1 * L2.get_right() is similar to |L1 x L2| (if L1 and L2 are coplanar) 
}
/*
 *double operations:
 * 4 additions
 * 2 subtractions
 * 6 multiplications
 * 1 division
 */

//get coefficient required to find intercept on a circle
/*
 * i.e.
 * given the line p' = p + xv
 * and the circle |p' - o| = r
 * we get:
 * (p + xv - o)^2 = r^2
 * expand and get
 * x^2.v^2 + 2x.(v.(p-o)) + (p-o)^2 - r ^2 = 0
 * a quadratic of x
 * this is solved algorithmically below:
 */
pair<double, double> line::coefficients (point centre, double rsqr) //note radius squared is asked for
{
    point tocentre = p - centre; // vector from line to centre
    double dirsqr = dir.square(); // |dir|^2 is used twice, and is the 'a' coefficient in the quadratic
    double cross = dir * tocentre.get_right(); // this cross product is part of a factorisation of a difference of two squares ( (a.b)^2 - (|a|.|b|)^2 )
    double delta = dirsqr * rsqr - cross * cross; // this is a quarter of the b^2 - 4ac 'delta' term in the quadratic
    double sqrtdelta = sqrt(delta); // half of sqrt (b^2 - 4ac)
    double b = dir * tocentre; // half of the b term in the quadratic
    //since b and sqrtdelta are both half of what they actually represent, doubling them cancels out with the 2 in the denominator,
    // giving x = (b +- sqrtdelta) / a
    double alpha = (-b - sqrtdelta) / dirsqr;
    double beta = (-b + sqrtdelta) / dirsqr;
    return pair<double, double> (alpha, beta);
}
/*
 * not factorising the cross product gives the expected quarter-delta expression below:
 * delta = b * b - dirsqr * (tocentre.square() - rsqr)
 * 
 * would this have been less computation anyway?
 * it eliminates cross from necessity so this balances
 * the tocentre.square() as both are roughly inner products
 * so it becomes two multiplications and two differences
 *
 * versus the above two multiplications, one difference
 * negligable difference, so the neat form wins. (cross product)
 */

line produce (point const &a, point const &b)
    {return {a, b - a};}

    } // end namespace

vector operator- (line L, point P)
{
    double c (L.dir.get_right() * (L.p - P)); // L.dir.get_right() is perpendicular to the line, so c is equal to distance(P, L) * |v|
    return c * (L.dir.get_right()) / L.dir.square(); // result is by definition perpendicular to the line, and c * L.dir.get_right() will be this direction but off by L^2
}

