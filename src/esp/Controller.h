#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include <Arduino.h>
#include <time.h>

#include "BotHandler.h"
#include "WiFiConfiguration.h"
#include "Types.h"

class SoftwareSerial;

class Controller
{
private:
    BotHandler *botHandler;
    WiFiConfiguration *wifiConfiguration;
    SoftwareSerial mySerial;

    time_t rawtime;
public:
    Controller();
    ~Controller();

    void begin(BotHandler*, WiFiConfiguration*);
    void start();
};

#endif