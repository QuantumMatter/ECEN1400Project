//
//  SI7021.cpp
//  RPiMaster
//
//  Created by David Kopala on 12/5/17.
//  Copyright © 2017 David Kopala. All rights reserved.
//

#include "SI7021.hpp"

SI7201::SI7201(int address) {
    addr = address;
    
    fd = wiringPiI2CSetup(address);;
}

void SI7201::update() {
    wiringPiI2CWrite(fd, 0xF5);
    delay(0.3);
    
    char data[2] = {0};
    if(read(fd, data, 2) != 2) {
        cout<<"SI7021 IO Error"<<endl;
    }
    /*char data0 = wiringPiI2CRead(fd);
    char data1 = wiringPiI2CRead(fd);*/
    
    humidity = ((data[0] * 256 + data[1]) * 125 / 65536.0) - 6;
    
    delay(0.3);
    wiringPiI2CWrite(fd, 0xF3);
    delay(0.3);
    
    /*data0 = wiringPiI2CRead(fd);
    data1 = wiringPiI2CRead(fd);*/
    if(read(fd, data, 2) != 2) {
        cout<<"SI7021 IO Error"<<endl;
    }
    
    celcius = ((data[0] * 256 + data[1]) * 175.72 / 65536.0) - 46.85;
}
