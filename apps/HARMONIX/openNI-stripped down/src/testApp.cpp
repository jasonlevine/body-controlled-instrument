#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {


	nearThreshold = 500;
	farThreshold  = 1000;



	ofBackground(0, 0, 0);
    
    hardware.setup();				// libusb direct control of motor, LED and accelerometers
	hardware.setLedOption(LED_OFF); // turn off the led just for yacks (or for live installation/performances ;-)
    
    
	openNIContext.setup();	// all nodes created by code -> NOT using the xml config file at all
	//openNIContext.setupUsingXMLFile();
	openNIDepth.setup(&openNIContext);
	openNIImage.setup(&openNIContext);
    
	openNIUser.setup(&openNIContext);
	openNIUser.setSmoothing(0.8);				// built in openni skeleton smoothing...
	openNIUser.setUseMaskPixels(false);
	openNIUser.setUseCloudPoints(false);
	openNIUser.setMaxNumberOfUsers(1);					

    
	openNIContext.toggleRegisterViewport();
	openNIContext.toggleMirror();
    
    
    //audio
    seq.start();

    sampler = ofxAudioUnitSampler('aumu', 'dls ', 'appl');
    sampler.setChannel(0);
    sampler.setBank(0, 0);
    sampler.setProgram(6);
    sampler >> output;
    output.start();
    
    note = lastNote = 0;
    cursor = lastCursor = ofVec2f(0, 0);
    cursorVel = maxCursVel = 0;
    //cursorTail.setColor(ofColor(100, 100, 25));
    
    stats = false;
    
    int scaleDegrees[] = {0, 2, 4, 5, 7, 9, 11};
    major.assign(scaleDegrees, scaleDegrees+7);
    
    ofEnableAlphaBlending();
    
    seqBallCol = ofColor(200, 150, 230);
    

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
    
    lastCursor = cursor;
    
    // cursor
    if (trackedUser) {
        
        
        squeeze.update(trackedUser->neck.position[0].X, trackedUser->neck.position[0].Y, 
                       trackedUser->right_lower_torso.position[1].X, trackedUser->right_lower_torso.position[1].Y,
                       trackedUser->left_lower_arm.position[1].X, trackedUser->left_lower_arm.position[1].Y,
                       trackedUser->right_lower_arm.position[1].X, trackedUser->right_lower_arm.position[1].Y);
        
        /*   float neckHipDiff = abs(trackedUser->neck.position[0].Y - trackedUser->right_lower_torso.position[1].Y);
        cursor.x = ofMap(trackedUser->right_lower_arm.position[1].X, 
                  trackedUser->neck.position[0].X, 
                  trackedUser->neck.position[0].X + neckHipDiff, 0, ofGetWidth());
        
        cursor.y = ofMap(trackedUser->right_lower_arm.position[1].Y, 
                  trackedUser->neck.position[0].Y, 
                  trackedUser->right_lower_torso.position[1].Y, 0, ofGetHeight());
        
        
        ofVec2f deltaC = cursor - lastCursor;
        cursorVel = deltaC.length();
        if (cursorVel > maxCursVel && stats) maxCursVel = cursorVel;
        
        cursorTail.append(cursor);
        cursorTail.update();*/
    }

    
    //blooms.update();
    
    // audio
    /*
    int sub = ofMap(cursorVel, 0, 30, 1, 4);
    sub*=4;
    seq.setSubdivision(sub);
    */
    seq.update();
    
    seqBall.x = squeeze.left.x + seq.getCurrentBeat() * (squeeze.rightLeftDiff.x/8) + seq.getDiffN() * (squeeze.rightLeftDiff.x/8);
    seqBall.y = squeeze.left.y + seq.getCurrentBeat() * (squeeze.rightLeftDiff.y/8) + seq.getDiffN() * (squeeze.rightLeftDiff.y/8);
    seqBallCol.setBrightness((1 - seq.getDiffN()) * 255);

    if (seq.getDiffN() == 0) {
        if (lastNote) sampler.midiNoteOff(lastNote, 0);
        int box = 14 - float(seqBall.y) / ofGetHeight() * 14;
        if (box < 7) note = major[box%7] + 60;
        else note = major[box%7] + 72;
        int vel = 127; 
        sampler.midiNoteOn(note, vel);
        lastNote = note;
        blooms.addBloom(seqBall, ofColor(200, 50, 100));
    }
    
    blooms.update();
    
}

//--------------------------------------------------------------
void testApp::draw(){

	ofSetColor(255, 255, 255);

	glPushMatrix();
    
    blooms.draw();
    //cursorTail.draw();
    
    squeeze.draw();
    ofSetColor(255);
    ofNoFill();
    ofCircle(seqBall.x, seqBall.y, 20);
    
    ofSetColor(seqBallCol);
    ofFill();
    ofCircle(seqBall.x, seqBall.y, 20);
   
	glPopMatrix();

	ofSetColor(255, 255, 0);


	string statusSmoothSkel = ofToString(openNIUser.getSmoothing());
    stringstream msg;

	msg
	<< "beat: " << ofToString(seq.getCurrentBeat()) << endl
    << "maxcursorVEl : " << ofToString(maxCursVel, 2) << endl
    << "( / ) : smooth skely (openni) : " << statusSmoothSkel << endl
    << "- / + : nearThreshold         : " << ofToString(nearThreshold) << endl
	<< "< / > : farThreshold          : " << ofToString(farThreshold) << endl
	<< endl
	<< "FPS   : " << ofToString(ofGetFrameRate()) << "  " << endl;

	ofDrawBitmapString(msg.str(), 20, 560);

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


		case '9':
		case '(':
			smooth = openNIUser.getSmoothing();
			if (smooth - 0.1f > 0.0f) {
				openNIUser.setSmoothing(smooth - 0.1f);
			}
			break;
		case '0':
		case ')':
			smooth = openNIUser.getSmoothing();
			if (smooth + 0.1f <= 1.0f) {
				openNIUser.setSmoothing(smooth + 0.1f);
			}
			break;
		
		case '>':
		case '.':
			farThreshold += 50;
			if (farThreshold > openNIDepth.getMaxDepth()) farThreshold = openNIDepth.getMaxDepth();
			break;
		case '<':
		case ',':
			farThreshold -= 50;
			if (farThreshold < 0) farThreshold = 0;
			break;

		case '+':
		case '=':
			nearThreshold += 50;
			if (nearThreshold > openNIDepth.getMaxDepth()) nearThreshold = openNIDepth.getMaxDepth();
			break;

		case '-':
		case '_':
			nearThreshold -= 50;
			if (nearThreshold < 0) nearThreshold = 0;
			break;
		case 'r':
			openNIContext.toggleRegisterViewport();
			break;
            
        case 's':
            seq.start();
            break;
            
        case 'S':
            seq.stop();
            break;
            
        case 'g':
            stats = !stats;
            
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
    sampler.midiNoteOn(60, 127);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

