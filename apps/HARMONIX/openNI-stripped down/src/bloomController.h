//
//  bloomController.h
//  HMXprototype3
//
//  Created by Jason Levine on 12-06-18.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
#include "bloom.h"

class bloomController {
public:
    bloomController(){};
    void update();
    void draw();
    void addBloom(const ofVec2f, const ofColor col);
    
protected:
    list<bloom> blooms;
};

