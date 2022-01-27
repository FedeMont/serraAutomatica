#define DEBUG

#include "Energia.h"
// Include application, user and local libraries
#include "SPI.h"

#include "Display.h"
Display display;

#include "Navigator.h"
Navigator navigator;

#include "MyClock.h"
MyClock myClock;

#include "Controller.h"
Controller controller;

#include "SoilSensor.h"
SoilSensor soilSensor;
#include <Wire.h> // Needed by Energia for Tiva C LaunchPad

volatile bool hasReceivedMessage = false;

void setup()
{
	// Serial.begin(115200);

	// attachInterrupt(5, read1, CHANGE); // funziona
	// attachInterrupt(3, serialEventRun1, CHANGE); // non funziona
	// display.begin();

	// navigator.begin();

	controller.begin(&display, &navigator, &myClock, &soilSensor);
}

void loop()
{
	controller.start();
	
	if (hasReceivedMessage)
	{
		controller.readFromESP(Serial1.readStringUntil('@'));
		hasReceivedMessage = false;
		// char c = char(Serial1.read());
        // Serial.print(c + " ");

        // if (c == '@') {
		// 	completeMessage.trim();
        //     Serial.println(" complete: " + completeMessage);
		// 	controller.readFromESP(completeMessage);

		// 	hasReceivedMessage = false;
		// 	completeMessage = "";
        // } else {
        //     completeMessage += c;
        // }
	}
	
	// if (hasReceivedMessage) {
	// 	controller.readFromESP();
	// 	hasReceivedMessage = false;
	// }
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
