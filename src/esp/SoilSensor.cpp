#include "SoilSensor.h"

#undef DEBUG

SoilSensor::SoilSensor()
{
    this->maxValue = 1023;
    this->minValue = 0;
    this->dryThreshold = (3 * (this->maxValue - this->minValue) / 5);
    this->wetThreshold = (2 * (this->maxValue - this->minValue) / 5);
}

SoilSensor::~SoilSensor()
{
}

int SoilSensor::readSensor(int pin)
{
    int value = analogRead(pin);
#ifdef DEBUG
    Serial.println("Pin:" + String(pin));
    Serial.println("Soil: " + String(value));
#endif
    return value;
}

bool SoilSensor::shouldWatering(int sensorValue)
{
    if (sensorValue > this->dryThreshold)
        this->lastShouldWatering = true;
    else if (sensorValue < this->wetThreshold)
        this->lastShouldWatering = false;

#ifdef DEBUG
    Serial.println("lastShouldWatering: " + String(this->lastShouldWatering));
#endif
    return this->lastShouldWatering;
}

float SoilSensor::valueToPercentage(int value)
{
    float percentage = (100 * (float)value) / (float)(this->maxValue - this->minValue);
    return 100.0 - percentage;
}