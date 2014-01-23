//
//  boundingBox.h
//  HMXprototype3
//
//  Created by Jason Levine on 12-06-18.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "ofMain.h"

class boundingBox {
public:
    boundingBox(){};
    
    boundingBox(const int _x1, const int _y1, const int _x2, const int _y2)
    : x1(_x1), y1(_y1), x2(_x2), y2(_y2) {};
    
    void setBounds(const int _x1, const int _y1, const int _x2, const int _y2)
    { x1 = _x1; y1 = _y1; x2 = _x2; y2 = _y2; };
    
    bool within(const int x, const int y)
    { if (x > x1 && x < x2 && y > y1 && y < y2) return true;
        else return false; };

    
    void draw(){ ofRect(x1, y1, x2 - x1, y2 - y1); };
    
protected:
    int x1, y1, x2, y2; 
};
