#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "Energia.h"
#include "display.h"
#include "navigator.h"
#include "myClock.h"
#include "types.h" 

class Controller {
public:
    Controller();

    void begin(Display*, Navigator, MyClock*);
    void chooseTime(Action);

    void start();

private:
    Display* display;
    Navigator navigator;
    MyClock* myClock;
};

#endif
