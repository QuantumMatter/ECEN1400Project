//
//  ArduinoSlave.hpp
//  RPiMaster
//
//  Created by David Kopala on 12/3/17.
//  Copyright © 2017 David Kopala. All rights reserved.
//

#ifndef ArduinoSlave_hpp
#define ArduinoSlave_hpp

#include <stdio.h>
#include <wiringPiI2C.h>

class ArduinoSlave {
    
private:
    int addr;
    int fd;
    int data;
    
public:
    ArduinoSlave(int address);
    int getAddr() {
        return addr;
    };
    int getData() {
        return data;
    };
    
    void read();
    
};

#endif /* ArduinoSlave_hpp */
