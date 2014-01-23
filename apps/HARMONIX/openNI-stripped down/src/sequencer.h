//
//  sequencer.h
//  HMXprototype1
//
//  Created by Jason Levine on 12-06-08.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
#include "ofMain.h"
#include "ofxTimer.h"

class sequencer {
public:
    sequencer();
    sequencer(const int bpm, const int timeSig);
    void update();
    void draw();
    
    void start();
    void stop() { playing = false; };
    void setSubdivision(const int sub);
    int getCurrentBeat() { return currentBeat; };
    bool bang() { return clock.alarm(); };
    float getDiffN() { return clock.getDiffN(); };
  
protected:
    ofxTimer clock;
    bool playing;
    int tempo;
    int beatLength;
    int currentBeat;
    int beatsPerMeasure;

};

