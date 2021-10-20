#include "Energia.h"
#include "navigator.h"

Navigator::Navigator()
{
    this->joystickX = 2;
    this->joystickY = 26;
    this->joystickSelect = 5;
    this->buttonUp = 33;
    this->buttonDown = 32;
}

void Navigator::begin() {
    pinMode(this->joystickSelect, INPUT_PULLUP);
    pinMode(this->buttonUp, INPUT_PULLUP);
    pinMode(this->buttonDown, INPUT_PULLUP);
}

Action Navigator::getAction()
{
    Action action_t = NONE;
    bool action[7] = {analogRead(this->joystickY) > 682,
                        analogRead(this->joystickY) < 341,
                        analogRead(this->joystickX) < 341,
                        analogRead(this->joystickX) > 682,
                        !digitalRead(this->joystickSelect),
                        !digitalRead(this->buttonUp),
                        !digitalRead(this->buttonDown)};

    for (int i = 0; i < 7; i++)
    {
        if (action[i])
        {
            action_t = Action(i);
            break;
        }
        
    }

    return action_t;
}