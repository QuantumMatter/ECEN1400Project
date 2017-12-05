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
    
    fd = wiringPiI2CSetup(address);
    
    int file;
    char *bus = "/dev/i2c-1";
    if((file = open(bus, O_RDWR)) < 0)
    {
        printf("Failed to open the bus. \n");
        exit(1);
    }
    // Get I2C device, TSL2561 I2C address is 0x60(96)
    ioctl(file, I2C_SLAVE, addr);
}

void SI7201::update() {
    cout<<""<<endl;
    //wiringPiI2CWrite(fd, 0xF5);
    uint8_t config[1] = {0xF5};
    write(file, config, 1);
    delay(0.3);
    
    uint8_t data[2] = {0};
    if(read(file, data, 2) != 2) {
        cout<<"SI7021 IO Error"<<endl;
    }
    /*char data0 = wiringPiI2CRead(fd);
    char data1 = wiringPiI2CRead(fd);*/
    
    humidity = ((data[0] * 256 + data[1]) * 125 / 65536.0) - 6;
    
    delay(0.3);
    config[0] = {0xF3};
    //wiringPiI2CWrite(fd, 0xF3);
    write(file, config, 1);
    delay(0.3);
    
    /*data0 = wiringPiI2CRead(fd);
    data1 = wiringPiI2CRead(fd);*/
    if(read(file, data, 2) != 2) {
        cout<<"SI7021 IO Error"<<endl;
    }
    
    celcius = ((data[0] * 256 + data[1]) * 175.72 / 65536.0) - 46.85;
}
