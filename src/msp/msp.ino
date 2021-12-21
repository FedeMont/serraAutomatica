#define DEBUG

#include "Energia.h"
// Include application, user and local libraries
#include "SPI.h"

#include "Display.h"
Display display;

#include "navigator.h"
Navigator navigator;

#include "MyClock.h"
MyClock myClock;

#include "Controller.h"
Controller controller;

#include "soilSensor.h"
SoilSensor soilSensor; 
#include <Wire.h>      // Needed by Energia for Tiva C LaunchPad 


void setup() {
  // Serial.begin(115200);

  // attachInterrupt(5, read1, CHANGE); // funziona
  // attachInterrupt(3, serialEventRun1, CHANGE); // non funziona

  // display.begin();

  // navigator.begin();

  controller.begin(&display, &navigator, &myClock, &soilSensor);
}

void loop() {
  controller.start();
}

void serialEvent1() {
  controller.readFromESP();
}
