#include "Energia.h"
#include "soilSensor.h"

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
    Serial.println(value);
    return value;
}

bool SoilSensor::shouldWatering(int sensorValue)
{
    if (sensorValue > this->dryThreshold) this->lastShouldWatering = true;
    else if (sensorValue < this->wetThreshold) this->lastShouldWatering = false;
    
    Serial.print("lastShouldWatering: ");
    Serial.println(this->lastShouldWatering);
    return this->lastShouldWatering;
}