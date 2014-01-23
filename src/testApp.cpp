#include "testApp.h"
//'aumu', 'CaC2', 'CamA'

ofxAudioUnitFilePlayer kick;
ofxAudioUnitSampler wobble('aumu', 'CaC2', 'CamA');


int seqCounter = 0;
int numBeats = 1;
int note = 0;
//int lastNote = 0;
bool wobbleOn = false;
bool kickOn = false;
bool leftPlaying = false;
bool rightPlaying = false;
int  leftNotePlaying = 0;
int  rightNotePlaying = 0;
ofVec3f leftHand(0,0,0);
vector<int> minorScale;

bool setupComplete = false;

Poco::Timestamp::TimeDiff hdelay = 125000; // delay between 2 events, in microseconds
Poco::Timestamp::TimeDiff nextTime = 0;    // time when next event occurs (calculated in seqTimerFunc)

void seqTimerFunc(Poco::Timestamp::TimeDiff curTime);
void seqTimerFunc(Poco::Timestamp::TimeDiff curTime)
// Events that are regularly sent are processed here
{
    if(setupComplete){
        if(curTime >= nextTime) {
            nextTime += hdelay;
                        
            if (seqCounter % numBeats == 0 && kickOn) {
                //kick.stop();
                kick.play();
                
            }
            
            if (seqCounter % numBeats == 0 && wobbleOn ) {
//                if (leftHand.z < -350) {

                    note = 52 + minorScale[int(leftHand.y * 6)];
                    
                    if(leftNotePlaying != note) {
                        wobble.midiNoteOff(leftNotePlaying, 0);
                        leftNotePlaying = note;
                        wobble.midiNoteOn(leftNotePlaying, 80);
                    }
                
//                    if(leftPlaying == false) {
//                        leftNotePlaying =  note;
//                        wobble.midiNoteOn(leftNotePlaying, 80);
//                        leftPlaying = true;
//                    }
                    
//                }
                
//                if (leftHand.z >= -350 && leftPlaying == true) {
//                    leftPlaying = false;
//                    wobble.midiNoteOff(leftNotePlaying, 0);
//                }
            }
            
            seqCounter++;
        }
    }
}



//--------------------------------------------------------------
void testApp::setup() {
    
    //audiounit setup
    
    wobble.loadCustomPreset("presets/resonant_evil_wobble.aupreset");
    //wobble.setParameter(kAudioUnitParameterUnit_BPM, kAudioUnitScope_Global, 120);
    
    drone = ofxAudioUnitSampler('aumu', 'CaC2', 'CamA');
    drone.loadCustomPreset("presets/friendlyfire.aupreset");
    
    kick.setFile(ofToDataPath("audio/hardcorekick.wav"));
    
    bandpass = ofxAudioUnit(kAudioUnitType_Effect, kAudioUnitSubType_BandPassFilter);
    bandpass.setParameter(kBandpassParam_Bandwidth, kAudioUnitScope_Global, 600);
    
    delay = ofxAudioUnit(kAudioUnitType_Effect, kAudioUnitSubType_Delay);
    delay.setParameter(kDelayParam_WetDryMix, kAudioUnitScope_Global, 50);
    
    reverb = ofxAudioUnit(kAudioUnitType_Effect, kAudioUnitSubType_MatrixReverb);
    //everb.setParameter(kReverbParam_DryWetMix, kAudioUnitScope_Global, 0);
    reverb.setParameter(kReverbParam_SmallLargeMix, kAudioUnitScope_Global, 80);
    
    compressor = ofxAudioUnit('aufx', 'mcmp', 'appl');
    compressor.setParameter(kMultibandCompressorParam_Threshold1, kAudioUnitScope_Global, -10.0);
    compressor.setParameter(kMultibandCompressorParam_Headroom1, kAudioUnitScope_Global, 0.1);
    compressor.setParameter(kMultibandCompressorParam_Threshold2, kAudioUnitScope_Global, -10.0);
    compressor.setParameter(kMultibandCompressorParam_Headroom2, kAudioUnitScope_Global, 0.1);
    compressor.setParameter(kMultibandCompressorParam_Threshold3, kAudioUnitScope_Global, -10.0);
    compressor.setParameter(kMultibandCompressorParam_Headroom3, kAudioUnitScope_Global, 0.1);
    compressor.setParameter(kMultibandCompressorParam_Threshold4, kAudioUnitScope_Global, -10.0);
    compressor.setParameter(kMultibandCompressorParam_Headroom4, kAudioUnitScope_Global, 0.1);

    //signal chain
    kick >> bandpass >> drumTap;
    wobble >> wobbleLevel >> bassTap;
    voice >> voiceTap >> voiceLevel >> delay >> reverb >> fxTap;
    
    mixer.setInputBusCount(4);
    drone.connectTo(mixer, 0);
    drumTap.connectTo(mixer, 1);
    bassTap.connectTo(mixer, 2);
    fxTap.connectTo(mixer, 3);
    mixer.setInputVolume(0.6, 0);
    mixer.setInputVolume(0.8, 1);
    mixer.setInputVolume(1.0, 2);
    mixer.setInputVolume(0.8, 3);
    
    mixer >> compressor >> output;
    
    voice.start();
    output.start();
    
    //soundplayer
    technoDrop.loadSound("audio/techno-drop.wav");
    bassDrop.loadSound("audio/bass-drop.wav");
    glassShattering.loadSound("audio/glass-object.aif");
    
    
    //midi notes
    int scaleDegrees[] = {0, 2, 3, 5, 7, 8, 11};
    minorScale.assign(scaleDegrees, scaleDegrees+7);
    
    // interaction variables
    leftPlaying = rightPlaying = false;
    leftNotePlaying = rightNotePlaying = 0;
    
    note = 35;
    numBeats = 8;
    beatLocked = false;
    
    lastLeftZ = predictiveZ = 0;
    
    reverbOn = false;
    showDone = false;
    
    // timer
	timer = new Timer(0, 10); // parameters : immediate and delay of 10 milliseconds (fast enough i think)
	timer->start(TimerCallback<seqTimer>(sTimer, &seqTimer::onTimer), Thread::PRIO_HIGHEST);
    
    //graphics
    ofEnableAlphaBlending();
	ofBackground(0, 0, 0);
    ofSetFrameRate(60);
    ofSetVerticalSync(true);

	//open NI
    hardware.setup();
	    
	openNIContext.setup();	
	openNIDepth.setup(&openNIContext);
	openNIImage.setup(&openNIContext);
    
	openNIUser.setup(&openNIContext);
	openNIUser.setSmoothing(0.8);				// built in openni skeleton smoothing...
	openNIUser.setUseMaskPixels(true);
	openNIUser.setUseCloudPoints(false);
	openNIUser.setMaxNumberOfUsers(1);					

	openNIContext.toggleRegisterViewport();
	openNIContext.toggleMirror();
    
    
        
    alfabet.loadFont("fonts/Alfphabet-IV.ttf", 30);
    calibrate = true;
    
    setupComplete = true;
    
    //cv
    contourFinder.setMinAreaRadius(10);
	contourFinder.setMaxAreaRadius(200);
    contourFinder.setThreshold(127);
    
    voiceSamples.assign(512, 0.0);
    fxSamples.assign(512, 0.0);
    drumSamples.assign(512, 0.0);
    bassSamples.assign(512, 0.0);
    
    //postprocessing
    post.init(1920, 1080);
    post.createPass<FxaaPass>()->setEnabled(true);
    post.createPass<DofPass>()->setEnabled(true);
    post.createPass<BloomPass>()->setEnabled(true);

            
}


//--------------------------------------------------------------
void testApp::update(){
    hardware.update();
    
    // update all nodes
    openNIContext.update();
    openNIDepth.update();
    openNIImage.update();
    openNIUser.update();
    trackedUser = openNIUser.getTrackedUser(1);
    
    voiceTap.getSamples(voiceSamples);
    fxTap.getSamples(fxSamples);
    bassTap.getSamples(bassSamples);
    drumTap.getSamples(drumSamples);

    
    float leftX = ofMap(trackedUser->left_lower_arm.position[1].X, trackedUser->hip.position[0].X, trackedUser->hip.position[0].X - abs(trackedUser->hip.position[0].Y - trackedUser->neck.position[0].Y), 0.0, 1.0, true);
    
    float leftY = ofMap(trackedUser->left_lower_arm.position[1].Y, trackedUser->hip.position[0].Y, trackedUser->neck.position[0].Y, 0.0, 1.0, true);
    float leftZ = trackedUser->left_lower_arm.position[1].Z-trackedUser->neck.position[0].Z;
    
    predictiveZ = leftZ + (leftZ - lastLeftZ);
    lastLeftZ = leftZ;
    
    leftHand.set(leftX, leftY, leftZ);
    
    float rightX = ofMap(trackedUser->right_lower_arm.position[1].X, trackedUser->hip.position[0].X, trackedUser->hip.position[0].X + abs(trackedUser->hip.position[0].Y - trackedUser->neck.position[0].Y), 0.0, 1.0, true);
    
    float rightY = ofMap(trackedUser->right_lower_arm.position[1].Y, trackedUser->hip.position[0].Y, trackedUser->neck.position[0].Y, 0.0, 1.0, true);
    
    float rightZ = trackedUser->right_lower_arm.position[1].Z-trackedUser->neck.position[0].Z;
    
    rightHand.set(rightX, rightY, rightZ);
    
    head.set(trackedUser->neck.position[1].X,
             480 - trackedUser->neck.position[1].Y,
             trackedUser->neck.position[1].Z);

    if (calibrate) {
        if (trackedUser->left_lower_arm.position[1].X > 0  &&
            trackedUser->left_lower_arm.position[1].Y > 0 ) {
            calibrate = false;
            glassShattering.play();
            drone.midiNoteOn(52, 127);
        }
    }
    else {
        ofImage mask;
        mask.allocate(640,480, OF_IMAGE_GRAYSCALE);
        mask.setFromPixels(openNIUser.getUserPixels(1), 640, 480, OF_IMAGE_GRAYSCALE); //
        
        contourFinder.findContours(mask);
        
        contoursVoice = contourFinder.getPolylines();
        contoursFx = contourFinder.getPolylines();
        contoursDrums = contourFinder.getPolylines();
        contoursBass = contourFinder.getPolylines();
        for (int i = 0; i < contoursVoice.size(); i++) {
            contoursVoice[i] = contoursVoice[i].getResampledByCount(512);
            contoursFx = contoursDrums = contoursBass = contoursVoice;
            for (int j = 0; j < 511; j++) {
                ofVec2f diff = contoursVoice[i][j+1] - contoursVoice[i][j];
                ofVec2f normal = ofVec2f(-diff.y, diff.x);
                normal.normalize();
                contoursVoice[i][j] += normal * voiceSamples[j] * 200;
                contoursFx[i][j] += normal * fxSamples[j] * 200;
                contoursDrums[i][j] += normal * drumSamples[j] * 200;
                contoursBass[i][j] += normal * bassSamples[j] * 200;
            }
        }

        
        float remixX = ofMap(head.x, 185, 500, 0.0, 1.0, true);
        drone.setParameter(16, kAudioUnitScope_Global, remixX);
        
        float remixZ = ofMap(head.z, 3000, 2000, 0.0, 1.0, true);
        drone.setParameter(17, kAudioUnitScope_Global, remixZ);

        if (head.y < 160 && !showDone) {
            drone.midiNoteOff(52, 0);
            bassDrop.play();
            technoDrop.play();
            //mixer.setInputVolume(0.0, 0);
            mixer.setInputVolume(0.0, 1);
            mixer.setInputVolume(0.0, 2);
            mixer.setInputVolume(0.0, 3);
            showDone = true;
        }
        
        if (head.z > 1400) {  // 2300 videology
            if (head.x < 310) { 
                backLeftQuadrant();
            }
            else { 
                backRightQuadrant();
            }
        }
        else { //if head/z < 2500

            voiceVol = ofMap(leftHand.z, -350, -500, 0.0, 1.0, true);
            voiceLevel.setInputVolume(voiceVol);
            
            if (head.x < 310) {
                frontLeftQuadrant();
            }
            else { 
                frontRightQuadrant();
            }
            
            delay.setParameter(kDelayParam_DelayTime, kAudioUnitScope_Global, delayTime);
            float feedback = ofMap(delayTime, 0.001, 1.0, 93, 50, true);
            delay.setParameter(kDelayParam_Feedback, kAudioUnitScope_Global, feedback);
            float lowpass = ofMap(leftHand.x, 0.0, 1.0, 20000, 100, true);
            delay.setParameter(kDelayParam_LopassCutoff, kAudioUnitScope_Global, lowpass);
        }
    }
}

//--------------------------------------------------------------
void testApp::frontLeftQuadrant(){
    if (wobbleOn) wobbleOn = false;
    delayTime = ofMap(leftHand.y, 0.0, 1.0, 0.02, 0.001, true);
    if (reverbOn) {
        reverb.setParameter(kReverbParam_DryWetMix, kAudioUnitScope_Global, 0);
        reverbOn = false;
    }
}

//--------------------------------------------------------------
void testApp::frontRightQuadrant(){
    if (!reverbOn) {
        reverb.setParameter(kReverbParam_DryWetMix, kAudioUnitScope_Global, 80);
        reverbOn = true;
    }
    
    if (leftHand.y > 0.8) {
        delayTime = 1.0;
    }
    else if (leftHand.y > 0.6 && leftHand.y < 0.8) {
        delayTime = 0.5;
    }
    else if (leftHand.y > 0.4 && leftHand.y < 0.6) {
        delayTime = 0.3333;
    }
    else if (leftHand.y > 0.2 && leftHand.y < 0.4) {
        delayTime = 0.25;
    }
    else if (leftHand.y < 0.2) {
        delayTime = 0.125;
    }
//    float drywet = ofMap(leftHand.z, -350, -500, 0, 100);
//    reverb.setParameter(kReverbParam_DryWetMix, kAudioUnitScope_Global, drywet);
}

//--------------------------------------------------------------
void testApp::backLeftQuadrant(){
    if (!wobbleOn) {
        wobbleOn = true;
    }
    float wobbleVol = ofMap(leftHand.z, -350, -500, 0.0, 1.0, true);
    wobbleLevel.setInputVolume(wobbleVol);
//    wobble.setVolume(wobbleVol);
    
    wobble.setParameter(16, kAudioUnitScope_Global, leftHand.y);
    wobble.setParameter(17, kAudioUnitScope_Global, leftHand.x);    
}

//--------------------------------------------------------------
void testApp::backRightQuadrant(){
    if (wobbleOn) wobbleOn = false;
    
    if (trackedUser->left_upper_leg.position[1].Y < trackedUser->left_upper_leg.position[0].Y && kickOn)
        beatLocked = true;
    
    if (trackedUser->right_upper_leg.position[1].Y < trackedUser->right_upper_leg.position[0].Y)
        beatLocked = false;
    
    
    if (!beatLocked) {
        if (leftHand.y > 0.8) {
            numBeats = 8;
        }
        if (leftHand.y > 0.6 && leftHand.y < 0.8) {
            numBeats = 4;
        }
        if (leftHand.y > 0.4 && leftHand.y < 0.6) {
            numBeats = 3;
        }
        if (leftHand.y > 0.2 && leftHand.y < 0.4) {
            numBeats = 2;
        }
        if (leftHand.y < 0.2) {
            numBeats = 1;
        }
        
        kickOn = (leftHand.z < -350) ? true : false;
        
        if (rightHand.z < -350) {
            float center_freq = 60 + rightHand.y * 5000;
            bandpass.setParameter(kBandpassParam_CenterFrequency, kAudioUnitScope_Global, center_freq);
            
            float bandwidth = 100 + rightHand.x * 11000;
            bandpass.setParameter(kBandpassParam_Bandwidth, kAudioUnitScope_Global, bandwidth);
            mixer.setInputVolume(0.9 - rightHand.x / 3.0, 1);
        }
        
    }
   
}


//--------------------------------------------------------------
void testApp::draw(){
    if (calibrate) {
        ofSetColor(255, 255, 255);

        openNIDepth.draw();
        openNIUser.draw();
        
        
        ofLine(0, 475, 640, 475);
        ofLine(0, 450, 640, 450);
        ofLine(0, 425, 640, 425);
        ofLine(0, 400, 640, 400);
        
        ofSetColor(255, 255, 0);
        float zDiff = trackedUser->left_lower_arm.position[1].Z - trackedUser->neck.position[0].Z;
        
        stringstream msg;

        msg
        << "FPS   : " << ofToString(ofGetFrameRate()) << "  " << endl
        << endl
        << "head.x : " << ofToString(head.x, 0) << endl
        << "head.y : " << ofToString(head.y, 0) << endl
        << "head.z : " << ofToString(head.z, 0) << endl
        << "left.z : " << ofToString(leftHand.z, 0) << endl
        << "leftpredictive: " << ofToString(predictiveZ, 0) << endl
        << "leftPlaying : " << ofToString(leftPlaying) << endl
        << "wobbleOn : " << ofToString(wobbleOn) << endl
        << "note : " << ofToString(note) << endl
        << endl
        << "left knee : " << ofToString(trackedUser->left_upper_leg.position[1].Y) << endl
        << "left hip : " << ofToString(trackedUser->left_upper_leg.position[0].Y) << endl
        << "beatLocked : " << ofToString(beatLocked) << endl
        << "delaytime:" << ofToString(delayTime, 4) << endl ;
    //    << "left.x : " << ofToString(leftHand.x) << endl
    //    << "left.y : " << ofToString(leftHand.y) << endl
    //    << "right.x : " << ofToString(rightHand.x) << endl
    //    << "right.y : " << ofToString(rightHand.y) << endl
    //    << "numBeats : " << numBeats << endl;

        alfabet.drawString(msg.str(), 680, 40);
    }
    else {
//        openNIUser.draw();
//        contourFinder.draw();
        ofSetFullscreen(true);
        post.begin();
        ofPushMatrix();
        ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
        ofScale(ofGetHeight()/480. / 3. * 4, ofGetHeight()/480.);
        ofTranslate(-640/2, -480/2);
        for (int i = 0; i < contoursVoice.size(); i++) {
            
           
            int drumAlpha = ofMap(drumTap.getRMS(0), 0.0, 0.1, 0, 255);
            ofSetColor(122,155,215, drumAlpha);
            ofFill();
            ofBeginShape();
            for (int j = 0; j < contoursDrums[i].size(); j++) {
                ofVertex(contoursDrums[i][j]);
            }
            ofEndShape();
            
            
            int bassAlpha = ofMap(bassTap.getRMS(0), 0.0, 0.1, 0, 255);
            ofSetColor(255,0,155, bassAlpha);
            ofFill();
            ofBeginShape();
            for (int j = 0; j < contoursBass[i].size(); j++) {
                ofVertex(contoursBass[i][j]);
            }
            ofEndShape();
            
            int fxAlpha = ofMap(fxTap.getRMS(0), 0.0, 0.1, 0, 255);
            ofSetColor(0,255,155, fxAlpha);
            ofFill();
            ofBeginShape();
            for (int j = 0; j < contoursFx[i].size(); j++) {
                ofVertex(contoursFx[i][j]);
            }
            ofEndShape();
            
            int voiceAlpha = ofMap(voiceTap.getRMS(0), 0.0, 0.1, 0, 255);
            ofSetColor(255,155,155, voiceAlpha);
            ofFill();
            ofBeginShape();
            for (int j = 0; j < contoursVoice[i].size(); j++) {
                ofVertex(contoursVoice[i][j]);
            }
            ofEndShape();
            
            

            
            
//            contoursVoice[i].draw();
//            ofSetColor(155,255,155);
//            contoursFx[i].draw();
//            ofSetColor(155,155,255);
//            contoursDrums[i].draw();
//            ofSetColor(155,0,155);
//            contoursBass[i].draw();
        }
        ofPopMatrix();
        post.end();
    }
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){

	float smooth;

	switch (key) {

		case 357: // up key
			hardware.setTiltAngle(hardware.tilt_angle++);
			break;
		case 359: // down key
			hardware.setTiltAngle(hardware.tilt_angle--);
			break;
            
        case 'w':
            wobble.showUI();
            
                    
		default:
			break;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

