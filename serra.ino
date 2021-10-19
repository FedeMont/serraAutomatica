 /*This example shows how to mix colors with an RGB LED using the analogWrite() function
 together with random() function to get the random color effect.
 
 The circuit utilizes the LEDs on the Educational BoosterPack MK II:
 * RED LED attached from digital pin 39 to ground.
 * GREEN LED attached from digital pin 38 to ground.
 * BLUE LED attached from digital pin 37 to ground.
 
 Created 3 Dec 2013
 By Dung Dang
 
 http://energia.nu/RandomSeed.html
 http://energia.nu/AnalogWrite.html 
 
 This example code is in the public domain.
 
 */

#include <Wire.h>      // Needed by Energia for Tiva C LaunchPad 
#include "OPT3001.h"
#define USE_USCI_B1 
opt3001 opt3001;

const uint8_t backlightPin = 39; //PWM-capable pin tied to backlight
const uint16_t darkestLUX = 0; 
const uint16_t brightestLUX = 4000;

const uint8_t dimmestBacklight = 1;
const uint8_t brightestBacklight = 254;

// Core library for code-sense
#if defined(WIRING) // Wiring specific
#include "Wiring.h"
#elif defined(MAPLE_IDE) // Maple specific
#include "WProgram.h"
#elif defined(MPIDE) // chipKIT specific
#include "WProgram.h"
#elif defined(DIGISPARK) // Digispark specific
#include "Arduino.h"
#elif defined(ENERGIA) // LaunchPad MSP430, Stellaris and Tiva, Experimeter Board FR5739 specific
#include "Energia.h"
#elif defined(CORE_TEENSY) // Teensy specific
#include "WProgram.h"
#elif defined(ARDUINO) && (ARDUINO >= 100) // Arduino 1.0 and 1.5 specific
#include "Arduino.h"
#elif defined(ARDUINO) && (ARDUINO < 100) // Arduino 23 specific
#include "WProgram.h"
#else // error
#error Platform not defined
#endif

// Include application, user and local libraries
#include "SPI.h"

// Screen selection
#define HX8353E // HX8353E K35_SPI

#if defined(K35_SPI)
#include "Screen_K35_SPI.h"
Screen_K35_SPI myScreen;

#elif defined(HX8353E)
#include "Screen_HX8353E.h"
Screen_HX8353E myScreen;

#else
#error Unknown screen
#endif

int ledRed = 39, ledBlue = 37, ledGreen = 38, ledWhite = 19;    // LED connected to digital pin 9
uint32_t readings;

int coloursNumber = 8;
uint16_t colours[8] = {whiteColour, redColour, orangeColour, yellowColour, greenColour, cyanColour, blueColour, violetColour};
uint16_t colour;

void setupLUX() {
  opt3001.begin(); 
  Serial.println("OPT3001 Initialized!");
}

void readLUX() {
  readings = opt3001.readResult();
  
  Serial.print("LUX Readings = ");
  Serial.println(readings, DEC);
}

void setupLCD() {
  Serial.println("*** LCD_protocol");
  
  myScreen.begin();
  Serial.println(myScreen.WhoAmI());
  Serial.print(myScreen.screenSizeX(), DEC);
  Serial.print("x");
  Serial.println(myScreen.screenSizeY(), DEC);
  
  myScreen.setFontSize(myScreen.fontMax());
  myScreen.clear(blackColour);
}

void writeLCD(String text) {
  colour = colours[random(coloursNumber - 2) + 1];
//    myScreen.clear(blackColour);
  myScreen.dRectangle(0, 0, myScreen.screenSizeX(), myScreen.screenSizeY(), colour);

  myScreen.setFontSolid(true);
  myScreen.setPenSolid(false);
  myScreen.setOrientation(0);
  myScreen.setFontSize(0);

  myScreen.setPenSolid(true);
  myScreen.dRectangle(4, 4, myScreen.screenSizeX() - 8, 10, blackColour);
  myScreen.setPenSolid(false);
  myScreen.gText(4, 4, "LUX: " + text, whiteColour);
  
  delay(50);
}

void setup()  { 
  // Initialize the pseudo-random number generator
  Serial.begin(115200);
  delay(1000);

  setupLCD();
  setupLUX();
  
  randomSeed(1);
} 

void loop()  { 

  readLUX();
  writeLCD(String(constrain(map(readings, darkestLUX, brightestLUX, dimmestBacklight, brightestBacklight), dimmestBacklight, brightestBacklight)));
 
  // sets the value (range from 0 to 255) for 3 RGB channels:
  analogWrite(ledRed, constrain(map(readings, darkestLUX, brightestLUX, dimmestBacklight, brightestBacklight), dimmestBacklight, brightestBacklight));
  analogWrite(ledBlue, constrain(map(readings, darkestLUX, brightestLUX, dimmestBacklight, brightestBacklight), dimmestBacklight, brightestBacklight));
  analogWrite(ledGreen, constrain(map(readings, darkestLUX, brightestLUX, dimmestBacklight, brightestBacklight), dimmestBacklight, brightestBacklight));
  analogWrite(ledWhite, constrain(map(readings, darkestLUX, brightestLUX, dimmestBacklight, brightestBacklight), dimmestBacklight, brightestBacklight));   
  // wait for 30 milliseconds before changing the color again
//  delay(300);               

  
  delay(300);

}
