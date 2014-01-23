//
//  sequencer.cpp
//  HMXprototype1
//
//  Created by Jason Levine on 12-06-08.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "sequencer.h"

sequencer::sequencer() {
    tempo = 60;
    beatsPerMeasure = 8;
    beatLength = 60000 / tempo / 4;
    currentBeat = 0;
}

sequencer::sequencer(const int bpm, const int timeSig) {
    tempo = bpm;
    beatsPerMeasure = timeSig;
    beatLength = 60000 / tempo / 4;
    currentBeat = 0;
}

void sequencer::update() {
    if (playing && clock.alarm()) {
        currentBeat++;
        currentBeat%=beatsPerMeasure;
        clock.setAlarm(beatLength);
    }
}


void sequencer::start() { 
    clock.setAlarm(beatLength);
    playing = true; 
}

void sequencer::setSubdivision(const int sub) {
    beatLength = 60000 / tempo / sub;
    beatsPerMeasure = sub;
}