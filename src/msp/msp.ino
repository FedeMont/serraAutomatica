#define DEBUG

#include "Energia.h"
// Include application, user and local libraries
#include "SPI.h"

#include "display.h"
Display display;

#include "navigator.h"
Navigator navigator;

#include "myClock.h"
MyClock myClock;

#include "Controller.h"
Controller controller;

#include "soilSensor.h"
SoilSensor soilSensor; 

#include <Wire.h>      // Needed by Energia for Tiva C LaunchPad 

int coloursNumber = 8;
uint16_t colours[8] = {whiteColour, redColour, orangeColour, yellowColour, greenColour, cyanColour, blueColour, violetColour};
uint16_t colour;

void setup() {
  Serial.begin(115200);

  // attachInterrupt(5, read1, CHANGE); // funziona
  // attachInterrupt(3, serialEventRun1, CHANGE); // non funziona

  display.begin();

  navigator.begin();

  controller.begin(&display, navigator, &myClock, &soilSensor);
}

void loop() {
  controller.start();
}

void serialEvent1() {
  controller.readFromESP();
}
