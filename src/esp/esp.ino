#include <Arduino.h>
#include "Controller.h"
Controller controller;

void setup()
{
    controller.begin();
}

void loop()
{
    controller.start();
}