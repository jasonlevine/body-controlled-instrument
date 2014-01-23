//
//  handPath.h
//  openNIdatamine
//
//  Created by Jason Levine on 12-05-31.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
#include "ofMain.h"


class handPath {
    
public:
    handPath();
    handPath(const ofColor color);
    void update();
    void draw();
    void append(const ofVec2f point);
    void setColor(const ofColor color){ pathColor = color; };

protected:
    vector<ofVec2f>     handPosHistory;
    vector<float>       alpha;
    ofColor           pathColor;
};
