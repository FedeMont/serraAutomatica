#define DEBUG

#include <Energia.h>
// Include application, user and local libraries
#include "SPI.h"

#include "Display.h"
Display display;

#include "Navigator.h"
Navigator navigator;

#include "MyClock.h"
MyClock myClock;

#include "../include/Controller.h"
Controller controller;

#include "SoilSensor.h"
SoilSensor soilSensor;
#include <Wire.h> // Needed by Energia for Tiva C LaunchPad

volatile bool hasReceivedMessage = false;
long connection_timeout_timer = millis();

void setup()
{
	// attachInterrupt(5, read1, CHANGE); // funziona
	// attachInterrupt(3, serialEventRun1, CHANGE); // non funziona

	controller.begin(&display, &navigator, &myClock, &soilSensor);
}

void loop()
{
	controller.start();
	
	if (hasReceivedMessage)
	{
		controller.readFromESP(Serial1.readStringUntil('@'));
		hasReceivedMessage = false;
	}

	if (!controller.getConnectionState() && millis() - connection_timeout_timer() > 120000) { // 2 minutes
		controller.hasConnectionTimedOut = true;
#ifdef DEBUG
    	Serial.println("Connection TimedOut");
#endif
	}
}

void uartInterrupt()
{
	hasReceivedMessage = true;
}

void serialEvent1()
{
	// controller.readFromESP();
	uartInterrupt();
}
