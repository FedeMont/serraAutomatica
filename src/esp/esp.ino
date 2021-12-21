#define TELEGRAM_DEBUG
#include <Arduino.h>
#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>

#include <NTPClient.h>
#include <WiFiUdp.h>
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 3600, 60000); // https://randomnerdtutorials.com/esp32-ntp-client-date-time-arduino-ide/

#include "SerialCommunication.h"
#include "BotHandler.h"
#include "WiFiConfiguration.h"
#include "Types.h"
#include "Controller.h"

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;

// #define MYTZ "CET-1CEST,M3.5.0,M10.5.0/3" // https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv

// Replace with your network credentials
const char *ssid = "TIM-19861131";
const char *password = "BussolaGay";

// SerialCommunication mySerial;
BotHandler botHandler = BotHandler(client);
WiFiConfiguration wifi(ssid, password);
Controller controller;

void setup()
{
	client.setInsecure();
	// client.setFingerprint("FC C9 84 21 5D 71 A1 A6 BF 17 C8 3A 8D 5E 8D E3 85 AE A9 11"); //api.telegram.org fingerprint generate from http://www.sha1-online.com/
	// client.setTrustAnchors(&cert);	  // Add root certificate for api.telegram.org
	controller.begin(&botHandler, &wifi, &timeClient);
}

void loop()
{
	controller.start();
}
