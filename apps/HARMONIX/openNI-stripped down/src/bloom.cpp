//
//  bloom.cpp
//  HMXprototype3
//
//  Created by Jason Levine on 12-06-18.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "bloom.h"

bloom::bloom(const ofVec2f _pos, const ofColor col){ 
    pos = _pos;
    vel = 1;
    decay = 0.99;
    size = 3;
    alpha = 1.0f;
    bloomColor = col;
    isDead = false;
}


void bloom::update() {
    if (alpha < 0) {
        isDead = true;
    }
    else {
        size+=vel;
        vel*=decay;
        alpha-=0.01;
        bloomColor.a = alpha * 255;
    }
}

void bloom::draw() {
    ofSetColor(bloomColor);
    ofCircle(pos.x, pos.y, size);
}
