// #include "Energia.h"
#include "TemperatureSensor.h"

TemperatureSensor::TemperatureSensor(/* args */)
{
    // this->maxValue = 20;
    // this->minValue = 10;
    this->hotThreshold = 25.0;// this->minValue + (3 * (this->maxValue - this->minValue) / 5);
    this->coldThreshold = 20.0;// this->minValue + (2 * (this->maxValue - this->minValue) / 5);

    this->lastShouldFan = false;
}

TemperatureSensor::~TemperatureSensor()
{
}

void TemperatureSensor::begin(){
    if (!this->tmp006.begin(TMP006_CFG_8SAMPLE)) {
        Serial.println("No sensor found");
    }
}

bool TemperatureSensor::shouldFan(float sensorValue)
{
    if (sensorValue > this->hotThreshold)
        this->lastShouldFan = true;
    else if (sensorValue < this->coldThreshold)
        this->lastShouldFan = false;

// #ifdef DEBUG
//     Serial.println("Fan: " + (int)this->lastShouldFan);
// #endif
    return this->lastShouldFan;
}

float TemperatureSensor::readObject() {
    return this->tmp006.readObjTempC();
}

float TemperatureSensor::readDiet() {
    return this->tmp006.readDieTempC();
}
