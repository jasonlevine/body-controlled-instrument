//
//  squeezebox.cpp
//  HMXprototype3
//
//  Created by Jason Levine on 12-06-19.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "squeezeBox.h"

squeezeBox::squeezeBox() {
    
}


void squeezeBox::update(const float neckX, const float neckY,
                        const float hipX, const float hipY,
                        const float leftHandX, const float leftHandY, 
                        const float rightHandX, const float rightHandY) {
    
    float neckHipDiff = abs(neckY - hipY);
    left.x =  ofMap(leftHandX, neckX, neckX - neckHipDiff, ofGetWidth()/2, 0);
    left.y = ofMap(leftHandY, neckY, hipY, 0, ofGetHeight());
    right.x = ofMap(rightHandX, neckX, neckX + neckHipDiff, ofGetWidth()/2, ofGetWidth());
    right.y = ofMap(rightHandY, neckY, hipY, 0, ofGetHeight());

    rightLeftDiff = right - left;
}


void squeezeBox::draw(){
    glLineWidth(6);
    ofCircle(left.x, left.y, 30);
    ofCircle(right.x, right.y, 30);
    ofSetColor(100);
    //glLineWidth(8);
    //ofLine(left.x, left.y, right.x, right.y);
    
}
