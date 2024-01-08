#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include <Arduino.h>
#include "Utils.h"
#include "WifiConfiguration.h"
#include "FileSystem.h"
#include "WebServer.h"

class Controller
{
private:
    FileSystem fs;
    WiFiConfiguration wifi;
    WebServer webServer;

    String net_ssid;
    String net_psw;
    int wateringTimerDelay;

    bool isConfigStatic;
    int ip[4];
    int default_gw[4];
    int subnet_mask[4];
    int dns[4];

    String telegram_chat_id;

    void deserializeJson(DynamicJsonDocument &);
public:
    Controller();
    ~Controller();

    bool begin();

    void start();
};

#endif