//
//  squeezeBox.h
//  HMXprototype3
//
//  Created by Jason Levine on 12-06-19.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
#include "ofMain.h"


class squeezeBox {
public:
    squeezeBox();
    void update(const float neckX, const float neckY,
                const float hipX, const float hipY,
                const float leftHandX, const float leftHandY,
                const float rightHandX,const float rightHandY);
    void draw();

    ofVec2f left, right, rightLeftDiff;
};