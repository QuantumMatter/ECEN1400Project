//
//  ArduinoSlave.cpp
//  RPiMaster
//
//  Created by David Kopala on 12/3/17.
//  Copyright © 2017 David Kopala. All rights reserved.
//

#include "ArduinoSlave.hpp"

#include <iostream>

ArduinoSlave::ArduinoSlave(int address) {
    addr = address;
    
    wiringPiSetup();
    pinMode(ARD_PIN, INPUT);
    
    fd = wiringPiI2CSetup(address);
}

void ArduinoSlave::read() {
    int type = digitalRead(ARD_PIN);
    
    data = wiringPiI2CRead(fd);
    if(type == 1) {
        data *= 3;
    }
}

void ArduinoSlave::write(int dat) {
    wiringPiI2CWrite(fd, data);
}
