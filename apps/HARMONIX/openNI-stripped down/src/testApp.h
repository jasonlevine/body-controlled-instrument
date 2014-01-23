#ifndef _TEST_APP
#define _TEST_APP

#include "ofxOpenNI.h"
#include "ofMain.h"
#include "boundingBox.h"
#include "sequencer.h"
#include "ofxAudioUnit.h"
#include "scaleRef.h"
#include "handPath.h"
#include "bloomController.h"
#include "squeezeBox.h"

class testApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);

	ofxOpenNIContext	openNIContext;
	ofxDepthGenerator	openNIDepth;
	ofxImageGenerator	openNIImage;
	ofxUserGenerator	openNIUser;
	ofxTrackedUser   *  trackedUser;
    ofxHardwareDriver	hardware;
    
	int					nearThreshold, farThreshold;
    
    //cursor
    ofVec2f             cursor, lastCursor;
    float               cursorVel;
    float               maxCursVel;
    
    // audio
    sequencer seq;
    ofxAudioUnitSampler sampler;
    ofxAudioUnitOutput output;
    
    int note, lastNote;
    
    bool stats;
    
    vector<int> major;
    vector<int> minor;
    vector<int> pentatonic;
    
    //handPath cursorTail;
    bloomController blooms;
    
    squeezeBox squeeze;
    ofVec2f seqBall;
    ofColor seqBallCol;

};

#endif
