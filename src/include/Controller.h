#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#ifdef Energia_h
#include <Energia.h>
#include "Display.h"
#include "Navigator.h"
#include "MyClock.h"
#include "SoilSensor.h"
#include "TemperatureSensor.h"
#else
#include <Arduino.h>
#include <NTPClient.h>
#include "BotHandler.h"
#include "WiFiConfiguration.h"
#endif
#include "SerialCommunication.h"
#include "Types.h"

class Controller
{
public:
    Controller();
    ~Controller();

#ifdef Energia_h
    void begin(Display *, Navigator *, MyClock *, SoilSensor *, TemperatureSensor *);
    void readFromESP(const String&);
#else
    void begin(BotHandler *, WiFiConfiguration *, NTPClient *);
    void readFromMSP(const String&);
    String getTime();
#endif
    void setDefaultValues();
    void start();
    void connectionTimeOut();

    bool hasConnectionTimedOut;
    bool getConnectionState();
    SerialCommunication mySerial;
private:
    void threeWayHandShake(const String &);

#ifdef Energia_h
    Display *display;
    Navigator *navigator;
    MyClock *myClock;
    SoilSensor *soilSensor;
    TemperatureSensor *temperatureSensor;

    State state;
    State previousState;
    Action previousSelectedAction;

    void wait();
    void sendEndMessage(const String &);

    void sendState(const String &);
    void sendModeState();
    void sendTimeState(const String &);
    void sendDayCycleState();
    void sendLightState(const String &);
    void sendSoilState();
    void sendWateringState(const String &);
    void sendTemperatureState();
    void sendFanState(const String &);

    void manualChooseState(Action);
    void changeState(State);
    void chooseTime(Action);
    void home();
    void manualStart(Action);
    void automaticStart();

    bool isConnectedToESP;

    bool isMinutePassed; // true for first clock write
    bool shouldWatering;
    bool shouldLight;
    bool shouldFan;

    bool hasSendDate;

    long wateringTimer;
#else
    BotHandler *botHandler;
    WiFiConfiguration *wifiConfiguration;
    NTPClient *timeAndDateClient;

    String infoMessage;
    State state;

    void stateMsgParser(Command);    

    bool isConnectedToMSP;
#endif
};

#endif
