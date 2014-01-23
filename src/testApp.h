#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxAudioUnit.h"
#include "ofxCv.h"
#include "ofxPostProcessing.h"
#include "openNIManager.h"

#include "Poco/Stopwatch.h"
#include "Poco/Thread.h"
#include "Poco/Timestamp.h"
#include "Poco/Timer.h"




using Poco::Stopwatch;
using Poco::Thread;
using Poco::Timer;
using Poco::TimerCallback;

class seqTimer
{
public:
    seqTimer()
    {
        stopwatch.start();
    }
	
    void onTimer(Timer& timer)
    {
		void seqTimerFunc(Poco::Timestamp::TimeDiff curTime); // prototype of my function
        seqTimerFunc(stopwatch.elapsed()); // function call
    }
	
private:
	Stopwatch stopwatch;
};


class testApp : public ofBaseApp{

public:
	void setup();
	void update();
    void frontLeftQuadrant();
    void frontRightQuadrant();
    void backLeftQuadrant();
    void backRightQuadrant();
	void draw();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
    
    openNIManager oni;
    
    // audio
    ofxAudioUnitInput   voice;
    ofxAudioUnitMixer   voiceLevel, wobbleLevel;
    ofxAudioUnitSampler drone;
    ofxAudioUnit        bandpass, delay, reverb;
    ofxAudioUnitMixer   mixer;
    ofxAudioUnit        compressor;
    ofxAudioUnitOutput  output;
    ofxAudioUnitTap     voiceTap, fxTap, drumTap, bassTap;
    
    //poco seq
    seqTimer sTimer;
	Timer * timer;

    ofTrueTypeFont      alfabet;
    
    bool                calibrate;
    bool                beatLocked;
    bool                reverbOn;
    bool                showDone;
    
    float               delayTime, voiceVol;
    
    ofSoundPlayer       technoDrop, bassDrop, glassShattering;
    
    //ofxCv
    ofxCv::ContourFinder contourFinder;
    
    //baudio
    vector<float> voiceSamples, fxSamples, drumSamples, bassSamples;
    vector<ofPolyline> contoursVoice, contoursFx, contoursDrums, contoursBass;
    
    ofxPostProcessing post;
};

#endif
