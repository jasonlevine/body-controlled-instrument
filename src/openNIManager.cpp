//
//  openNIManager.cpp
//  MdF_final
//
//  Created by Jason Levine on 2014-01-23.
//
//

#include "openNIManager.h"

void openNIManager::setup(){
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
    
    lastLeftZ = predictiveZ = 0;

}

void openNIManager::update(){
    hardware.update();
    
    // update all nodes
    openNIContext.update();
    openNIDepth.update();
    openNIImage.update();
    openNIUser.update();
    trackedUser = openNIUser.getTrackedUser(1);
    
    
    
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
}

void draw(){

}