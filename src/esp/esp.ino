#define TELEGRAM_DEBUG
#include <Arduino.h>
#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>

#include <NTPClient.h>
#include <WiFiUdp.h>
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 3600, 60000); // https://randomnerdtutorials.com/esp32-ntp-client-date-time-arduino-ide/

#include "BotHandler.h"
#include "WiFiConfiguration.h"
#include "Types.h"
#include "Controller.h"

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;

// Replace with your network credentials
const char *ssid = "Vodafone-MontagnaGuest";
const char *password = "MontagnaWiFi";
// const char *ssid = "FASTWEB-7HSG23";
// const char *password = "FG9KHU7JNH";

BotHandler botHandler = BotHandler(client);
WiFiConfiguration wifi(ssid, password);
Controller controller;

volatile bool hasReceivedMessage = false;
long connection_timeout_timer;

void setup()
{
	client.setInsecure();
	controller.begin(&botHandler, &wifi, &timeClient);
	connection_timeout_timer = millis();

	// attachInterrupt(digitalPinToInterrupt(13), uartInterrupt, CHANGE);
}

void loop()
{
	controller.start();
	if (hasReceivedMessage && !controller.hasConnectionTimedOut)
	{
		controller.readFromMSP(controller.mySerial.readStringUntil('@'));
		hasReceivedMessage = false;
	}

	if (!controller.getConnectionState() && (millis() - connection_timeout_timer > 120000))
	{ // 2 minutes == 120000
		controller.hasConnectionTimedOut = true;
#ifdef DEBUG
		Serial.println("Connection TimedOut");
#endif
	}

	serialEvent1();
}

ICACHE_RAM_ATTR void uartInterrupt()
{
	hasReceivedMessage = true;
}

void serialEvent1()
{
	if (controller.mySerial.available() > 0)
	{
		uartInterrupt();
	}
}