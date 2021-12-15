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

#include "controller.h"
Controller controller;

#include "soilSensor.h"
SoilSensor soilSensor; 

#include <Wire.h>      // Needed by Energia for Tiva C LaunchPad 
//#include "OPT3001.h"
//opt3001 opt3001;

uint32_t readOPT();
void writeLCD(String);

int coloursNumber = 8;
uint16_t colours[8] = {whiteColour, redColour, orangeColour, yellowColour, greenColour, cyanColour, blueColour, violetColour};
uint16_t colour;

unsigned long t_start = millis();

void setup() {
  // put your setup code here, to run once:

  Serial1.begin(115200);
  Serial.begin(115200);

  display.begin();

//  opt3001.begin(); 
  
  navigator.begin();

  controller.begin(&display, navigator, &myClock, &soilSensor);
}

void loop() {
  // controller.start();
  // Write
  if (Serial1.available()) {
    Serial.println(Serial1.readStringUntil('\n'));
  }

  // Read 
  // String f = "FUCK\n"; 
  // if (millis() - t_start > 1000) {
  //   Serial1.print(f);
  //   t_start = millis();
  // }
}

//uint32_t readOPT() {
//
//  uint32_t readings = opt3001.readResult();
//  Serial.print("LUX Readings = ");
//  Serial.println(readings, DEC);
//  
//  return readings;
//}
