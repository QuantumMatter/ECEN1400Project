#include "mpu6050.h"
#include <math.h>

MPU6050::MPU6050()
{
    fd = wiringPiI2CSetup(address);
    wiringPiI2CWriteReg8(fd, power_mgmt_1, 0);
}

void MPU6050::read()
{

    gyrox = readWord2C(gyro_x_out);
    gyroy = readWord2C(gyro_y_out);
    gyroz = readWord2C(gyro_z_out);

    accelx = readWord2C(accel_x_out);
    accely = readWord2C(accel_y_out);
    accelz = readWord2C(accel_z_out);

    accelxScaled = accelx / 16386.0;
    accelyScaled = accely / 16386.0;
    accelzScaled = accelz / 16386.0;

    xRot = getXRotation(accelxScaled, accelyScaled, accelzScaled);
    yRot = getYRotation(accelxScaled, accelyScaled, accelzScaled);

}

double MPU6050::distance(double a, double b)
{
    return sqrt((a*a) + (b*b));
}

double MPU6050::getXRotation(double x, double y, double z)
{
    double radians = atan2(y, distance(x, z));
    return (radians / M_PI) * -180;
}

double MPU6050::getYRotation(double x, double y, double z)
{
    double radians = atan2(x, distance(y, z));
    return (radians / M_PI) * -180;
}

double MPU6050::readWord(int reg)
{
    int high = wiringPiI2CReadReg8(fd, reg);
    int low = wiringPiI2CReadReg8(fd, reg+1);
    double val = (high << 8) + low;
    return val;
}

double MPU6050::readWord2C(int reg)
{
    double val = readWord(reg);
    if(val > 0x8000)
    {
        return -((65535 - val) + 1);
    }
    else
    {
        return val;
    }
}
