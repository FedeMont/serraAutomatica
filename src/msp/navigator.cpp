// #include "Energia.h"
#include "Navigator.h"

Navigator::Navigator()
{
    this->joystickX = 2;
    this->joystickY = 26;
    this->joystickSelect = 5;
    this->buttonUp = 33;
    this->buttonDown = 32;
    this->lightRelay = 19;
    this->fanRelay = 18;
    this->water = 27;
}

void Navigator::begin()
{
    pinMode(this->joystickSelect, INPUT_PULLUP);
    pinMode(this->buttonUp, INPUT_PULLUP);
    pinMode(this->buttonDown, INPUT_PULLUP);
    pinMode(this->lightRelay, OUTPUT);
    pinMode(this->fanRelay, OUTPUT);
    this->timer_start = millis();
}

enum Action Navigator::getAction()
{
    Action action_t = Action_NONE;

    if (millis() - timer_start > 300)
    {
        bool action[7] = {analogRead(this->joystickY) > 682,
                          analogRead(this->joystickY) < 200,
                          analogRead(this->joystickX) < 170, // rotto, va bene cosi
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

        this->timer_start = millis();
    }

    return action_t;
}

void Navigator::lightOn()
{
    this->fanOn();
    digitalWrite(this->lightRelay, HIGH);
}

void Navigator::lightOff()
{
    this->fanOff();
    digitalWrite(this->lightRelay, LOW);
}

void Navigator::fanOn()
{
    digitalWrite(this->fanRelay, HIGH);
}

void Navigator::fanOff()
{
    digitalWrite(this->fanRelay, LOW);
}

void Navigator::waterOn()
{
    digitalWrite(this->water, HIGH);
}

void Navigator::waterOff()
{
    digitalWrite(this->water, LOW);
}