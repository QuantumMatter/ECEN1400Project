#include "pca9685.h"
#include <stdlib.h>

PCA9685::PCA9685()
{
    fd = wiringPiI2CSetup(ADDRESS);
    setAllPwm(0, 0);
    wiringPiI2CWriteReg8(fd, MODE2, OUTDRV);
    wiringPiI2CWriteReg8(fd, MODE1, ALLCALL);
    
    mode1 = wiringPiI2CReadReg8(fd, MODE1);
    mode1 = mode1 + ~SLEEP;
    wiringPiI2CReadReg8(MODE1, mode1);
}

void PCA9685::setAllPwm(int on, int off)
{
    wiringPiI2CWriteReg8(fd, ALL_LED_ON_L, on & 0xFF);
    wiringPiI2CWriteReg8(fd, ALL_LED_ON_H, on >> 8);
    wiringPiI2CWriteReg8(fd, ALL_LED_OFF_L, off & 0xFF);
    wiringPiI2CWriteReg8(fd, ALL_LED_OFF_H, off >> 8);
}

void PCA9685::setPwmFreq(int freq_hz)
{
    double prescaleval = 25000000.0;
    prescaleval /= 4096.0;
    prescaleval /= (float) freq_hz;
    prescaleval -= 1.0;

    int prescale = floor(prescaleval + 0.5);
    int oldmode = wiringPiI2CReadReg8(fd, MODE1);
    double newMode = (oldmode & 0x7F) | 0x10;
    wiringPiI2CWriteReg8(fd, MODE1, newMode);
    wiringPiI2CWriteReg8(fd, PRESCALE, prescale);
    wiringPiI2CWriteReg8(fd, MODE1,  oldmode);
    wiringPiI2CWriteReg8(fd, MODE1, oldmode | 0x80);
}

void PCA9685::setPwm(int channel, int on, int off)
{
    wiringPiI2CWriteReg8(fd, LED0_ON_L+4*channel, on & 0xFF);
    wiringPiI2CWriteReg8(fd, LED0_ON_H+4*channel, on >> 8);
    wiringPiI2CWriteReg8(fd, LED0_OFF_L+4*channel, off & 0xFF);
    wiringPiI2CWriteReg8(fd, LED0_OFF_H+4*channel, off >> 8);
}
