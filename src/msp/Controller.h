#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "Energia.h"
#include "Display.h"
#include "Navigator.h"
#include "MyClock.h"
#include "SoilSensor.h"
#include "SerialCommunication.h"
#include "types.h"

class Controller
{
public:
    Controller();
    ~Controller();

    void begin(Display *, Navigator *, MyClock *, SoilSensor *);
    void start();

    void readFromESP(const String&);

    SerialCommunication mySerial;

private:
    Display *display;
    Navigator *navigator;
    MyClock *myClock;
    SoilSensor *soilSensor;

    Command lastCommandRecevied;
    State state;
    State previousState;
    Action previousSelectedAction;

    void sendState(String);
    void sendLightManual(String);
    void sendWaterManual(String);
    void sendFanManual(String);

    void chooseState(Action);
    void changeState(State);
    void chooseTime(Action);
    void home();
    void manualStart(Action);
    void automaticStart();

    bool isMinutePassed; // true for first clock write
    bool shouldWatering;

    bool hasReceivedState;
    bool hasReceivedLight;
    bool hasReceivedWater;
    bool hasReceivedFan;
    bool hasSendDate;

    bool lightManual;
    bool waterManual;
    bool fanManual;
};

#endif
