#ifndef __SOILSENSOR_H__
#define __SOILSENSOR_H__

#include "Energia.h"
#include "types.h"

class SoilSensor
{
private:
    uint8_t pin;

    int maxValue = 1023;
    int minValue = 0;
    int dryThreshold = (3*(maxValue-minValue)/5);
    int wetThreshold = (2*(maxValue-minValue)/5);

    bool lastShouldWatering;

public:
    SoilSensor(/* args */);
    ~SoilSensor();

    int readSensor();
    bool shouldWatering(int);
};

#endif
