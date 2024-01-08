#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include <Arduino.h>
#include "FileSystem.h"
#include "WiFiConfiguration.h"
#include "WebServer.h"
#include "BotHandler.h"
#include "Navigator.h"
#include "SoilSensor.h"
#include "MyClock.h"
#include "Types.h"

class Controller
{
private:
    FileSystem fileSystem;
    WiFiConfiguration wiFiConfiguration;
    WebServer webServer;
    BotHandler botHandler;
    Navigator navigator;
    SoilSensor soilSensor;
    MyClock myClock;

    int soilSensorPin;
    long manualWateringTimer;

    const char* net_ssid;
    const char* net_psw;
    int wateringTimerDelay;

    bool is_config_static;
    int ip[4];
    int default_gw[4];
    int subnet_mask[4];
    int dns[4];
    String telegram_chat_id;

    State lightState;
    State fanState;
    State waterState;

    void deserializeJson(DynamicJsonDocument &);

    void autoStart(DayCycle, bool);
public:
    Controller();
    ~Controller();

    void begin();

    void start();

    void getFreeHeap();
};

#endif //__CONTROLLER_H__
