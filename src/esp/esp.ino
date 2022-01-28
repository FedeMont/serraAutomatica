#define TELEGRAM_DEBUG
#include <Arduino.h>
#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>

#include <NTPClient.h>
#include <WiFiUdp.h>
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 3600, 60000); // https://randomnerdtutorials.com/esp32-ntp-client-date-time-arduino-ide/

#include "../include/SerialCommunication.h"
#include "BotHandler.h"
#include "WiFiConfiguration.h"
#include "../include/Types.h"
#include "../include/Controller.h"

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;

// #define MYTZ "CET-1CEST,M3.5.0,M10.5.0/3" // https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv

// Replace with your network credentials
const char *ssid = "TIM-19861131";
const char *password = "BussolaGay";

BotHandler botHandler = BotHandler(client);
WiFiConfiguration wifi(ssid, password);
Controller controller;

volatile bool hasReceivedMessage = false;
long connection_timeout_timer = millis();

void setup()
{
	client.setInsecure();
	// client.setFingerprint("FC C9 84 21 5D 71 A1 A6 BF 17 C8 3A 8D 5E 8D E3 85 AE A9 11"); //api.telegram.org fingerprint generate from http://www.sha1-online.com/
	// client.setTrustAnchors(&cert);	  // Add root certificate for api.telegram.org
	controller.begin(&botHandler, &wifi, &timeClient);

	// attachInterrupt(digitalPinToInterrupt(13), uartInterrupt, CHANGE);
}

void loop()
{
	controller.start();
	if (hasReceivedMessage) {
		controller.readFromMSP(controller.mySerial.readStringUntil('@'));
		hasReceivedMessage = false;
	}

	if (!controller.getConnectionState() && millis() - connection_timeout_timer() > 120000) { // 2 minutes
		controller.hasConnectionTimedOut = true;
#ifdef DEBUG
    	Serial.println("Connection TimedOut");
#endif
	}

	serialEvent1();
}

ICACHE_RAM_ATTR void uartInterrupt() {
	hasReceivedMessage = true;
}

void serialEvent1() {
	if (controller.mySerial.available() > 0) {
		uartInterrupt();
	}
}