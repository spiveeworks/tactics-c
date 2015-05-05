

struct copair {
	long x, y;
};

struct bounds {
	union {
		struct {long west, north, east, south;} dir;
		long el[4];
		struct {copair topleft, bottomright;} corner;
	};
    copair center () const {
        copair ret;
        ret.x = (dir.west + dir.east)/2;
        ret.y = (dir.north + dir.south)/2;
        return ret;
    }
    long width () const {return dir.east - dir.west;}
    long height () const {return dir.south - dir.north;}
};
//can cast directly to windows RECT, just as copair can cast directly to POINT

struct camera {
    Tact::point focus;
    double width, height;
    double tilt;
    
    bounds rec;
    /*
    SCG::vector x_dir () {return SCG::vector(width*cos(tilt), width*sin(tilt));}
    SCG::vector y_dir () {return SCG::vector(-height*sin(tilt), -height*cos(tilt));}
    */
    
    mutable double prevtilt;
    mutable SCG::vector xnorm;
    mutable SCG::vector ynorm;
    
    void check_dirs () {
        if (tilt != prevtilt)
        {
            prevtilt = tilt;
            xnorm = SCG::mkVector(tilt); //direction of x-axis
            ynorm = SCG::vector(-xnorm.y, xnorm.x);//now direction of y-axis
        }
    }

    camera (bounds nrec = bounds(),
        Tact::point nfocus = Tact::point (0.0, 0.0), 
        double nwidth = 100, double nheight = -100, double ntilt = 0.0) 
    {
        focus = nfocus;
        width = nwidth; height = nheight; tilt = ntilt;
        rec = nrec;
        
        prevtilt = tilt;
        xnorm = SCG::mkVector(tilt);
        ynorm = SCG::vector(-xnorm.y, xnorm.x);
        //as seen in void check_dirs();
    }

    long displaywidth  () {return rec.width() / 2;}
    long displayheight () {return rec.height() / 2;}
    copair displaycentre () {copair ret; ret.x = 512; ret.y = 350; return ret;}
    
    copair draw(Tact::point loc) /*const*/ {
        SCG::vector gap = loc - focus; //get vector from camera to loc
        check_dirs();
        double xR = gap * xnorm / width; //get the product of the cam-x_axis and the vector, and thence find the ratio of the component as to the screen width
        double yR = gap * ynorm / height; //same for cam-y axis and height
        
        //multiply ratios by the screen proportions, giving what should therefore be distances from the screen's centre
        xR *= displaywidth(); 
        yR *= displayheight();
        
        //cast and return
        copair ret = rec.center();
        ret.x += (long) xR;
        ret.y += (long) yR;
        
        return ret;
    }
    
    Tact::point locate(copair p) /*const*/ {
        copair cen = displaycentre();
        long relx = p.x - cen.x;
        long rely = p.y - cen.y;
        double xR = (double)relx / displaywidth();
        double yR = (double)rely / displayheight();
        
        return focus + (xR * xnorm * width) + (yR * ynorm * height);//Not yet tested, also maybe the radials should add together first before casting?
    }
    
};


copair* batchdraw(camera cam, std::vector<Tact::point> vertices) {
    copair* Vs = new copair [vertices.size()];
        //make sure you delete[] afterwards!
    for (unsigned i = 0; i < vertices.size(); i++)
        Vs[i] = cam.draw(vertices[i]);
    return Vs;
}


