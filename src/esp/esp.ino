#include "LittleFS.h"
#include <Arduino.h>

// #include "FileSystem.h"
// FileSystem fileSystem;

// #include "WiFiConfiguration.h"
// WiFiConfiguration wiFiConfiguration;

// #include "WebServer.h"
// WebServer webServer;

// #include "BotHandler.h"
// BotHandler botHandler;

// #include "Navigator.h"
// Navigator navigator;

// #include "SoilSensor.h"
// SoilSensor soilSensor;

// #include "MyClock.h"
// MyClock myClock;

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