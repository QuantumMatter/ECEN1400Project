#ifndef ESC_H
#define ESC_H

#include <QObject>
#include "pca9685.h"
#include <QThread>
#include <QTimer>

class ESC : public QObject
{
    Q_OBJECT
public:
    explicit ESC(int chan, QObject *parent = 0);
    bool ready;

private:
    PCA9685 *pwm;
    int channel;

signals:

public slots:
    void setMin();
    void setPower(int percent);
};

#endif // ESC_H
