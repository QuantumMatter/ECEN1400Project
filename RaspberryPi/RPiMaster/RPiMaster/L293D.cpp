//
//  L293D.cpp
//  RPiMaster
//
//  Created by David Kopala on 12/3/17.
//  Copyright © 2017 David Kopala. All rights reserved.
//

#include "L293D.hpp"

L293D::L293D(int pinF, int pinB, int type) {
    fPin = pinF;
    rPin = pinB;
    type = type;
    
    initStuff();
}

void L293D::initStuff(){
    wiringPiSetup();
    
    pinMode(fPin, OUTPUT);
    digitalWrite(fPin, 0);
    pinMode(rPin, OUTPUT);
    digitalWrite(rPin, 0);
}

void L293D::forward() {
    digitalWrite(rPin, 0);
    digitalWrite(fPin, 1);
}

void L293D::reverse() {
    digitalWrite(fPin, 0);
    digitalWrite(rPin, 1);
}

void L293D::stop() {
    digitalWrite(fPin, 0);
    digitalWrite(rPin, 0);
}


