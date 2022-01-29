#ifndef __SOILSENSOR_H__
#define __SOILSENSOR_H__

#include "Energia.h"
#include "Types.h"

class SoilSensor
{
private:
    uint8_t pin;

    int maxValue;
    int minValue;
    int dryThreshold;
    int wetThreshold;

    bool lastShouldWatering;

public:
    SoilSensor(/* args */);
    ~SoilSensor();

    int readSensor();
    bool shouldWatering(int);
    float valueToPercentage(int);
};

#endif
