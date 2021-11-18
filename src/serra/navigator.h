#ifndef __NAVIGATOR_H__
#define __NAVIGATOR_H__

#include "Energia.h"
#include "types.h" 

class Navigator
{
public:
    uint8_t joystickX, joystickY, joystickSelect, buttonUp, buttonDown, lightRelay, fanRelay, soilSensor;

    Navigator();

    void begin();
    Action getAction();
    void lightOn();
    void lightOff();
    void fanOn();
    void fanOff();
    int readMoisture();
    
private:
    long timer_start;
};

#endif
