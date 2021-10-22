#ifndef __NAVIGATOR_H__
#define __NAVIGATOR_H__

#include "Energia.h"
#include "enums.h" 

class Navigator
{
public:
    uint8_t joystickX, joystickY, joystickSelect, buttonUp, buttonDown;

    Navigator();

    void begin();
    Action getAction();
};

#endif
