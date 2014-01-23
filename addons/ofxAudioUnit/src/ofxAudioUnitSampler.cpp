//
//  ofxAudioUnitSampler.cpp
//
//  Created by Jason Levine on 12-06-11.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
#include "ofxAudioUnit.h"

// ----------------------------------------------------------
ofxAudioUnitSampler::ofxAudioUnitSampler(AudioComponentDescription description) 
// ----------------------------------------------------------
{
    _desc = description;
    initUnit();
}

// ----------------------------------------------------------
ofxAudioUnitSampler::ofxAudioUnitSampler(OSType type, 
                           OSType subType, 
                           OSType manufacturer)
// ----------------------------------------------------------
{
    _desc.componentType         = type;
    _desc.componentSubType      = subType;
    _desc.componentManufacturer = manufacturer;
    _desc.componentFlags        = 0;
    _desc.componentFlagsMask    = 0;
    initUnit();
};

// ----------------------------------------------------------
ofxAudioUnitSampler::ofxAudioUnitSampler(const ofxAudioUnitSampler &orig)
// ----------------------------------------------------------
{
	_desc = orig._desc;
    initUnit();
}

// ----------------------------------------------------------
ofxAudioUnitSampler& ofxAudioUnitSampler::operator=(const ofxAudioUnitSampler &orig)
// ----------------------------------------------------------
{
	if(this == &orig) return *this;
	
    _desc = orig._desc;
	_unit = orig._unit;
	
	return *this;
}


void ofxAudioUnitSampler::setBank(const UInt32 msb, const UInt32 lsb)
{
    MusicDeviceMIDIEvent(*_unit, 
                         kMidiMessage_ControlChange << 4 | midiChannelInUse, 
                         kMidiMessage_BankMSBControl, msb,
                         0/*sample offset*/);
    
    MusicDeviceMIDIEvent(*_unit, 
                         kMidiMessage_ControlChange << 4 | midiChannelInUse, 
                         kMidiMessage_BankLSBControl, lsb,
                         0/*sample offset*/);
}

void ofxAudioUnitSampler::setProgram(const UInt32 prog)
{
    MusicDeviceMIDIEvent(*_unit, 
                         kMidiMessage_ProgramChange << 4 | midiChannelInUse, 
                         prog, 0,
                         0/*sample offset*/);
}



void ofxAudioUnitSampler::midiNoteOn(const UInt32 note, const UInt32 vel){
    UInt32 noteOnCommand = 	kMidiMessage_NoteOn << 4 | midiChannelInUse;
    
    MusicDeviceMIDIEvent(	*_unit,
                         noteOnCommand,
                         note,
                         vel,
                         0);
}

void ofxAudioUnitSampler::midiNoteOff(const UInt32 note, const UInt32 vel){
    UInt32 noteOffCommand = kMidiMessage_NoteOff << 4 | midiChannelInUse;
    
    MusicDeviceMIDIEvent(	*_unit,
                         noteOffCommand,
                         note,
                         vel,
                         0);
}
