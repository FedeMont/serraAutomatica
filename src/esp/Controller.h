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
    NTPClient *timeAndDateClient;

    unsigned long lastTimeRead;
    int readDelay = 500;

    String infoMessage = "";

    State state = State_NONE;
    
public:
    Controller();
    ~Controller();

    SerialCommunication mySerial;
    String getTime();

    void begin(BotHandler *, WiFiConfiguration *, NTPClient *);
    void start();

    void readFromMSP(const String&);
};

#endif