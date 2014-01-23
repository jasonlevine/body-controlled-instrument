//
//  sequencer.cpp
//  HMXprototype1
//
//  Created by Jason Levine on 12-06-08.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "sequencer.h"

sequencer::sequencer() {
    tempo = 132;
    beatsPerMeasure = 16;
    beatLength = (60000 / tempo)/4;
    whole = half = quarter = eighth = sixteenth = 0;
    subdivision = 4;
}

sequencer::sequencer(const int bpm, const int timeSig) {
    tempo = bpm;
    beatsPerMeasure = timeSig;
    beatLength = (60000 / tempo)/4;
    whole = half = quarter = eighth = sixteenth = 0;

}

void sequencer::update() {
    if (playing) {
        if(sTimer.alarm()) {
            sixteenth++;
            sixteenth%=16;
            eighth = sixteenth / 2;
            quarter = eighth / 2;
            half = quarter / 2;
            whole = half / 2; 
            sTimer.setAlarm(beatLength);
            //cout << whole << " " << half << " " << quarter << " " << eighth << " " << sixteenth << endl;
        }
        if(eTimer.alarm()) {
            eTimer.setAlarm(beatLength*2);
        }
        if(qTimer.alarm()) {
            qTimer.setAlarm(beatLength*4);
        }
        if(hTimer.alarm()) {
            hTimer.setAlarm(beatLength*8);
        }
        if(wTimer.alarm()) {
            wTimer.setAlarm(beatLength*16);
        }
    }
}


void sequencer::start() { 
    sTimer.setAlarm(beatLength);
    eTimer.setAlarm(beatLength*2);
    qTimer.setAlarm(beatLength*4);
    hTimer.setAlarm(beatLength*8);
    wTimer.setAlarm(beatLength*16);
    playing = true; 
}

void sequencer::setSubdivision(const int sub) {
    subdivision = sub;
}

int sequencer::getCurrentBeat() {
    switch (subdivision) {
        case 1:
            return whole;
            break;
            
        case 2:
            return half;
            break;
        
        case 4:
            return quarter;
            break;
            
        case 8:
            return eighth;
            break;
            
        case 16:
            return sixteenth;
            break;
            
            
            
        default:
            break;
    }
    return quarter;
}

int sequencer::getBeats() {
    return subdivision;
}

float sequencer::getDiffA (){
    switch (subdivision) {
        case 1:
            return wTimer.getDiffA();
            break;
            
        case 2:
            return hTimer.getDiffA();
            break;
            
        case 4:
            return qTimer.getDiffA();
            break;
            
        case 8:
            return eTimer.getDiffA();
            break;
            
        case 16:
            return sTimer.getDiffA();
            break;
    }
}