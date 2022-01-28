#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "Energia.h"
#include "Display.h"
#include "Navigator.h"
#include "MyClock.h"
#include "SoilSensor.h"
#include "SerialCommunication.h"
#include "TemperatureSensor.h"
#include "types.h"

class Controller
{
public:
    Controller();
    ~Controller();

    void begin(Display *, Navigator *, MyClock *, SoilSensor *, TemperatureSensor *);
    void start();

    void readFromESP(const String&);

    SerialCommunication mySerial;

private:
    Display *display;
    Navigator *navigator;
    MyClock *myClock;
    SoilSensor *soilSensor;
    TemperatureSensor *temperatureSensor;

    Command lastCommandRecevied;
    State state;
    State previousState;
    Action previousSelectedAction;

    void wait();

    void sendState(String chatId);
    void sendMode();
    void sendDayCycle();
    void sendTime();
    void sendSoilSensor();
    void sendTemperatureSensor();
    void sendWater(String);
    void sendLight(String);
    void sendFan(String);
    void endMsg(String);

    void chooseState(Action);
    void changeState(State);
    void chooseTime(Action);
    void home();
    void manualStart(Action);
    void automaticStart();

    bool isMinutePassed; // true for first clock write

    bool shouldWatering;
    bool shouldFan;
    bool shouldLight;

    bool hasReceivedState;
    bool hasReceivedLight;
    bool hasReceivedWater;
    bool hasReceivedFan;
    bool hasSendDate;
};

#endif
