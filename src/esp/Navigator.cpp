#include "Navigator.h"

Navigator::Navigator()
{
    this->lightRelay = D5; // blue
    this->waterRelay = D6; // white
    this->fanRelay = D7; // green
    this->soilSensor = A0;
}

Navigator::~Navigator()
{
}

void Navigator::begin()
{
    pinMode(this->lightRelay, OUTPUT);
    pinMode(this->fanRelay, OUTPUT);
    pinMode(this->waterRelay, OUTPUT);
    pinMode(this->soilSensor, INPUT);
}

void Navigator::pinOn(int pin)
{
    digitalWrite(pin, HIGH);
}

void Navigator::pinOff(int pin)
{
    digitalWrite(pin, LOW);
}

int Navigator::getSoilSensorPin()
{
    return this->soilSensor;
}

void Navigator::lightOn()
{
    this->pinOn(this->lightRelay);
}

void Navigator::lightOff()
{
    this->pinOff(this->lightRelay);
}

void Navigator::waterOn()
{
    this->pinOn(this->waterRelay);
}

void Navigator::waterOff()
{
    this->pinOff(this->waterRelay);
}

void Navigator::fanOn()
{
    this->pinOn(this->fanRelay);
}

void Navigator::fanOff()
{
    this->pinOff(this->fanRelay);
}