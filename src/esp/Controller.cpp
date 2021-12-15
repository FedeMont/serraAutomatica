#include "Controller.h"
#include <SoftwareSerial.h>

Controller::Controller():mySerial(13, 15) // d7 rx bianco, d8 tx marrone
{
}

Controller::~Controller()
{
}

void Controller::begin(BotHandler *botHandler, WiFiConfiguration *wiFi)
{
    this->botHandler = botHandler;
    this->wifiConfiguration = wiFi;

    this->mySerial.begin(115200);

    this->botHandler->begin(&this->mySerial);
    this->wifiConfiguration->connect();

    this->mySerial.print("START\n");
}

void Controller::start()
{
    if (millis() > this->botHandler->lastTimeBotRan + this->botHandler->botRequestDelay)
    {
        int numNewMessages = this->botHandler->getUpdates();

        while (numNewMessages)
        {
#ifdef DEBUG
            Serial.println("got response");
#endif
            this->botHandler->handleNewMessages(numNewMessages);
            numNewMessages = this->botHandler->getUpdates();
        }

        this->botHandler->lastTimeBotRan = millis();
    }
}