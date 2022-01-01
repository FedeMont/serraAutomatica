#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include <Arduino.h>
#include <NTPClient.h>

#include "SerialCommunication.h"
#include "BotHandler.h"
#include "WiFiConfiguration.h"
#include "Types.h"

class Controller
{
private:
    BotHandler *botHandler;
    WiFiConfiguration *wifiConfiguration;
    SerialCommunication mySerial;
    NTPClient *timeAndDateClient;

    unsigned long lastTimeRead;
    int readDelay = 500;

public:
    Controller();
    ~Controller();

    String getTime();

    void begin(BotHandler *, WiFiConfiguration *, NTPClient *);
    void start();

    void readFromMSP();
};

#endif