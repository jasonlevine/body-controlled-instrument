//
//  openNIManager.h
//  MdF_final
//
//  Created by Jason Levine on 2014-01-23.
//
//

#pragma once
#include "ofVec3f.h"
#include "ofxOpenNI.h"


class openNIManager {
public:
    void setup();
    void update();
    void draw();
    
    ofxOpenNIContext	openNIContext;
	ofxDepthGenerator	openNIDepth;
	ofxImageGenerator	openNIImage;
	ofxUserGenerator	openNIUser;
	ofxTrackedUser   *  trackedUser;
    ofxHardwareDriver	hardware;
    
	int					nearThreshold, farThreshold;
    
    ofVec3f             leftHand, rightHand, head;
    float               lastLeftZ, predictiveZ;
    float               leftHandRelativePos;
};