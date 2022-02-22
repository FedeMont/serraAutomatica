#ifndef __NAVIGATOR_H__
#define __NAVIGATOR_H__

#include <Arduino.h>
#include "Types.h"

class Navigator
{
private:
    int lightRelay, waterRelay, fanRelay, soilSensor;

    void pinOn(int);
    void pinOff(int);
public:
    Navigator();
    ~Navigator();

    void begin();

    int getSoilSensorPin();

    void lightOn();
    void lightOff();
    void waterOn();
    void waterOff();
    void fanOn();
    void fanOff();
};

#endif //__NAVIGATOR_H__
