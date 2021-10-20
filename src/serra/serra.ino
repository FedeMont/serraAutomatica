#include "Energia.h"
// Include application, user and local libraries
#include "SPI.h"

//#include "navigator.h"
//Navigator navigator;

//#include "Screen_HX8353E.h"
//Screen_HX8353E myScreen;

#include "display.h"
Display display;

//#include "navigator.h"
Navigator navigator;

MyClock myClock;

#include <Wire.h>      // Needed by Energia for Tiva C LaunchPad 
#include "OPT3001.h"
opt3001 opt3001;

uint32_t readOPT();
void writeLCD(String);

int coloursNumber = 8;
uint16_t colours[8] = {whiteColour, redColour, orangeColour, yellowColour, greenColour, cyanColour, blueColour, violetColour};
uint16_t colour;

long time_start;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
//  delay(1000);
  
//  myScreen.begin();
//  myScreen.setFontSize(myScreen.fontMax());
//  myScreen.clear(blackColour);

  display.begin();

  opt3001.begin(); 
  time_start = millis();

  navigator.begin();
}

void loop() {
  if (millis() - time_start > 250) {
    
    display.chooseTime(navigator, &myClock);
    
    time_start = millis();
  }
  
}

uint32_t readOPT() {

  uint32_t readings = opt3001.readResult();
  Serial.print("LUX Readings = ");
  Serial.println(readings, DEC);
  
  return readings;
}

//void writeLCD(String text) {
//  colour = colours[random(coloursNumber - 2) + 1];
////    myScreen.clear(blackColour);
//  myScreen.dRectangle(0, 0, myScreen.screenSizeX(), myScreen.screenSizeY(), colour);
//
//  myScreen.setFontSolid(true);
//  myScreen.setPenSolid(false);
//  myScreen.setOrientation(0);
//  myScreen.setFontSize(0);
//
////  myScreen.setPenSolid(true);
////  myScreen.dRectangle(4, 4, myScreen.screenSizeX() - 8, 10, blackColour);
////  myScreen.setPenSolid(false);
//  myScreen.gText(4, 4, "LIUX:" + text, whiteColour);
//  
////  delay(50);
//}
