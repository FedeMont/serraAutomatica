#ifndef __TEMPERATURESENSOR_H__
#define __TEMPERATURESENSOR_H__

#include <Energia.h>
#include "Types.h"
#include <Wire.h>
#include "Adafruit_TMP006.h"

#define USE_USCI_B1 

class TemperatureSensor
{
public:
    TemperatureSensor(/* args */);
    ~TemperatureSensor();

    void begin();

    float readObject();
    float readDiet();
    
    bool shouldFan(float);
private:
    Adafruit_TMP006 tmp006;

    int maxValue;
    int minValue;
    float hotThreshold;
    float coldThreshold;

    bool lastShouldFan;
};

#endif
