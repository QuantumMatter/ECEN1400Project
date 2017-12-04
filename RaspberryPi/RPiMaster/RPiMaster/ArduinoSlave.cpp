//
//  ArduinoSlave.cpp
//  RPiMaster
//
//  Created by David Kopala on 12/3/17.
//  Copyright © 2017 David Kopala. All rights reserved.
//

#include "ArduinoSlave.hpp"

ArduinoSlave::ArduinoSlave(int address) {
    addr = address;
    
    fd = wiringPiI2CSetup(address);
}

void ArduinoSlave::read() {
    data = wiringPiI2CRead(fd);
}
