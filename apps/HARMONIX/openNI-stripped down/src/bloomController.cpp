//
//  bloomController.cpp
//  HMXprototype3
//
//  Created by Jason Levine on 12-06-18.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "bloomController.h"

void bloomController::update() {
    for (list<bloom>::iterator bb = blooms.begin(); bb != blooms.end(); ++bb) {
        bb->update();
        if (bb->isDead) {
            bb = blooms.erase(bb);
        }
    }
}

void bloomController::draw() {
    for (list<bloom>::iterator bb = blooms.begin(); bb != blooms.end(); ++bb) {
        bb->draw();
    }
}

void bloomController::addBloom(const ofVec2f pos, const ofColor col){
    bloom temp(pos, col);
    blooms.push_back(temp);
}
