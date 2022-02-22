#ifndef __SOILSENSOR_H__
#define __SOILSENSOR_H__

#include <Arduino.h>
#include "Types.h"

class SoilSensor
{
private:
    int maxValue;
    int minValue;
    int dryThreshold;
    int wetThreshold;

    bool lastShouldWatering;
public:
    SoilSensor();
    ~SoilSensor();

    int readSensor(int);
    bool shouldWatering(int);
    float valueToPercentage(int);

};

#endif
