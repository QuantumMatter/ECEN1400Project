#include "esc.h"

ESC::ESC(int chan, QObject *parent) : QObject(parent)
{
    channel = chan;
    ready = false;
    pwm = new PCA9685();
    pwm->setPwm(channel, 0, 200);
    QTimer::singleShot(2000, this, SLOT(setMin()));
}

void ESC::setMin()
{
    pwm->setPwm(channel, 0, 100);
    ready = true;
}

void ESC::setPower(int percent)
{
    if(ready)
    {
        pwm->setPwm(channel, 0, (percent + 100));
    }
}
