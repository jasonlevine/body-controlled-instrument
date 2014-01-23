//
//  sequencer.h
//  HMXprototype1
//
//  Created by Jason Levine on 12-06-08.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
#include "ofMain.h"
#include "ofxTimer.h"
/*
enum subdivision {
    whole = 1,
    half = 2,
    quarter = 3,
    eighth = 4,
    sixteenth = 5
};*/

class sequencer {
public:
    sequencer();
    sequencer(const int bpm, const int timeSig);
    void update();
    void draw();
    
    void start();
    void stop() { playing = false; };
    void setTempo(const int bpm) { tempo = bpm; };
    void setSubdivision(const int sub);
    void setBeatLength(const int duration) { beatLength = duration; };
    int getCurrentBeat();
    float getDiffA();
    int getBeats();
  
protected:
    ofxTimer wTimer, hTimer, qTimer, eTimer, sTimer;
    bool playing;
    int tempo;
    int barLength, beatLength;
    int whole, half, quarter, eighth, sixteenth;
    int beatsPerMeasure;
    int subdivision;
};

