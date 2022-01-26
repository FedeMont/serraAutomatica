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

    void readFromESP();

private:
    Display *display;
    Navigator *navigator;
    MyClock *myClock;
    SoilSensor *soilSensor;
    SerialCommunication mySerial;

    Command lastCommandRecevied;
    State state;
    State previousState;
    Action previousSelectedAction;

    void sendState(String);

    unsigned long lastTimeRead;
    int readDelay;

    void chooseState(Action);
    void changeState(State);
    void chooseTime(Action);
    void home();
    void manualStart(Action);
    void automaticStart();

    bool isMinutePassed; // true for first clock write
    bool isWatering;

};

#endif
