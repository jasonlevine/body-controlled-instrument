//
//  handPath.cpp
//  openNIdatamine
//
//  Created by Jason Levine on 12-05-31.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
#include "ofMain.h"
#include "handPath.h"

handPath::handPath() {
    pathColor = ofColor(255);
}

handPath::handPath(const ofColor color) {
    pathColor = color;
}

void handPath::update() {    
    for (int i = 0; i < alpha.size(); i++) {        
        if (alpha[i] < 0) {
            alpha.erase(alpha.begin() + i);
            handPosHistory.erase(handPosHistory.begin());
        }
        else {
            alpha[i]-=0.02;
        }
        
    }

}

void handPath::draw() {
    glLineWidth(6);
    glBegin(GL_LINE_STRIP);

    for (int i = 0; i < handPosHistory.size(); i++) {        
        glColor4f(float(pathColor.r) / 255, float(pathColor.g) / 255, float(pathColor.b) / 255, alpha[i]);
        glVertex2f(handPosHistory[i].x, handPosHistory[i].y);
    }
    glEnd();    
}

void handPath::append(const ofVec2f point) {    
    handPosHistory.push_back(point);
    alpha.push_back(1.0f);
}