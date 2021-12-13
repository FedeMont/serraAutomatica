#include "Energia.h"
#include "soilSensor.h"

#include "debugPrint.h"

SoilSensor::SoilSensor(/* args */)
{
    this->pin = 28;
}

SoilSensor::~SoilSensor()
{
}

int SoilSensor::readSensor()
{
    int value = analogRead(this->pin);
    DebugPrint().println(String(value));
    return value;
}

bool SoilSensor::shouldWatering(int sensorValue)
{
    if (sensorValue > this->dryThreshold) this->lastShouldWatering = true;
    else if (sensorValue < this->wetThreshold) this->lastShouldWatering = false;
    
    DebugPrint().print("lastShouldWatering: ");
    DebugPrint().println(String(this->lastShouldWatering));
    return this->lastShouldWatering;
}