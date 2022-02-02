#ifndef __NAVIGATOR_H__
#define __NAVIGATOR_H__

#include "Energia.h"
#include "Types.h"

class Navigator
{
public:
    Navigator();
    ~Navigator();

    void begin();

    enum Action getAction();
    
    void lightOn();
    void lightOff();
    void fanOn();
    void fanOff();
    void waterOn();
    void waterOff();

private:
    uint8_t joystickX, joystickY, joystickSelect, buttonUp, buttonDown,
        lightRelay, fanRelay, water;
    long timer_start;
};

#endif
