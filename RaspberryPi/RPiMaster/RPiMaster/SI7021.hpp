//
//  SI7021.hpp
//  RPiMaster
//
//  Created by David Kopala on 12/5/17.
//  Copyright © 2017 David Kopala. All rights reserved.
//

#ifndef SI7021_hpp
#define SI7021_hpp

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

using namespace std;

class SI7201 {
    
private:
    int addr;
    int file;
    
    float humidity;
    float celcius;
    
public:
    SI7201(int address = 0x40);
    void update();
    float getHumidity() {
        return humidity;
    };
    float getCelcius() {
        return celcius;
    };
};

#endif /* SI7021_hpp */
