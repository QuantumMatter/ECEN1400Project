//
//  MPL3115A2.hpp
//  RPiMaster
//
//  Created by David Kopala on 12/4/17.
//  Copyright © 2017 David Kopala. All rights reserved.
//

#ifndef MPL3115A2_hpp
#define MPL3115A2_hpp

#include <stdio.h>
#include <wiringPiI2C.h>
#include <iostream>

using namespace std;

class MPL3115A2 {
    
private:
    int addr;
    int fd;
    
    float pressure;
    float altitude;
    float celcius;
    
public:
    MPL3115A2(int address = 0x60);
    void read();
    float getPressure() {
        return pressure;
    }
    float getAltitude() {
        return altitude;
    }
    float getCelcius() {
        return celcius;
    }
};

#endif /* MPL3115A2_hpp */
