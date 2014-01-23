//
//  bloom.h
//  HMXprototype3
//
//  Created by Jason Levine on 12-06-18.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
#include "ofMain.h"

class bloom {
public:
    bloom(){};
    bloom(const ofVec2f _pos, const ofColor col);
    void update();
    void draw();
    bool isDead;
    
protected:
    ofVec2f pos;
    float vel;
    float decay;
    float size;
    float alpha;

    ofColor bloomColor;

};


