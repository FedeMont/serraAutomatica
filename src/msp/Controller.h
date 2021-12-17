#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "Energia.h"
#include "display.h"
#include "navigator.h"
#include "myClock.h"
#include "soilSensor.h"
#include "types.h"

class Controller
{
public:
    Controller();
    ~Controller();

    void begin(Display *, Navigator, MyClock *, SoilSensor *);
    void chooseTime(Action);

    void start();
    void readFromESP();

private:
    Display *display;
    Navigator navigator;
    MyClock *myClock;
    SoilSensor *soilSensor;

    Command lastCommandRecevied;
    State state = MANUAL;

    unsigned long lastTimeRead;
    int readDelay = 100;
    void writeToESP();

    void manualStart();
    void automaticStart();

    Action action;
    bool isMinutePassed = true; // true for first clock write
};

#endif
