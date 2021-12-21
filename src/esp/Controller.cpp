#include "Controller.h"
#include <SoftwareSerial.h>

Controller::Controller()
{
}

Controller::~Controller()
{
}

String Controller::getTime()
{
    this->timeAndDateClient->update();
    return this->timeAndDateClient->getFormattedDate();
}

void Controller::begin(BotHandler *botHandler, WiFiConfiguration *wiFi, NTPClient *timeClient)
{
    Serial.begin(115200);
    this->mySerial.begin(115200);
    pinMode(2, OUTPUT);

    this->botHandler = botHandler;
    this->wifiConfiguration = wiFi;
    this->timeAndDateClient = timeClient;

    this->botHandler->begin(&this->mySerial);
    this->wifiConfiguration->connect();
    this->timeAndDateClient->begin();

    Serial.println(this->getTime());

    // this->mySerial.write("/d" + buffer);
}

void Controller::start()
{
    this->botHandler->start();
}