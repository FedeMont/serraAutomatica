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

    bool hasConnectionTimedOut;
    SerialCommunication mySerial;

#ifdef Energia_h
    void begin(Display *, Navigator *, MyClock *, SoilSensor *, TemperatureSensor *);
    void readFromESP(const String&);

    void connectionTimeOut();
#else
    void begin(BotHandler *, WiFiConfiguration *, NTPClient *);
    void readFromMSP(const String&);

#endif
    bool getConnectionState();

    void start();
private:
#ifdef Energia_h
    Display *display;
    Navigator *navigator;
    MyClock *myClock;
    SoilSensor *soilSensor;
    TemperatureSensor *temperatureSensor;

    bool firstTimePowerOn;
    bool isConnectedToESP;

    bool isMinutePassed; // true for first clock write
    bool shouldWatering;
    bool shouldLight;
    bool shouldFan;

    bool isWaterAuto;
    bool isLightAuto;
    bool isFanAuto;

    int waterCounter;
    int lightCounter;
    int fanCounter;

    bool hasSentDate;

    long wateringTimer;

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

    void manualToggleLight();
    void manualToggleWater(const String &);
    void manualToggleFan();

    // void manualChooseState(Action);
    // void changeState(State);
    void chooseTime(Action);
    void automaticStart(Action);
#else
    BotHandler *botHandler;
    WiFiConfiguration *wifiConfiguration;
    NTPClient *timeAndDateClient;

    bool isConnectedToMSP;
    String infoMessage;

    String getTime(); 
    void stateMsgParser(Command);   

#endif

    void threeWayHandShake(const String &);
};

#endif
