//
//  MPL3115A2.cpp
//  RPiMaster
//
//  Created by David Kopala on 12/4/17.
//  Copyright © 2017 David Kopala. All rights reserved.
//

#include "MPL3115A2.hpp"

MPL3115A2::MPL3115A2(int address) {
    addr = address;
    fd = wiringPiI2CSetup(address);
    
    if (wiringPiI2CReadReg8(fd, WHO_AM_I) == 196) {
        cout<<"Altimeter online!"<<endl;
    }
    
    read();
}

void MPL3115A2::read() {
    
}
