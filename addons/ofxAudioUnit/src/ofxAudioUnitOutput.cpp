#include "ofxAudioUnit.h"

AudioComponentDescription outputDesc = {
	kAudioUnitType_Output,
	kAudioUnitSubType_HALOutput,
	kAudioUnitManufacturer_Apple
};

// ----------------------------------------------------------
ofxAudioUnitOutput::ofxAudioUnitOutput()
// ----------------------------------------------------------
{
  _desc = outputDesc;
  initUnit();
}

// ----------------------------------------------------------
bool ofxAudioUnitOutput::start()
// ----------------------------------------------------------
{
  return ERR_CHK_BOOL(AudioOutputUnitStart(*_unit), "starting output unit");
}

// ----------------------------------------------------------
bool ofxAudioUnitOutput::stop()
// ----------------------------------------------------------
{
  return ERR_CHK_BOOL(AudioOutputUnitStop(*_unit), "stopping output unit");
}
