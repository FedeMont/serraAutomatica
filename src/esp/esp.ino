#include <Arduino.h>
#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>

#include "BotHandler.h"
#include "WiFiConfiguration.h"
#include "Types.h"
#include "Controller.h"


X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;

// #include <ArduinoJson.h>

BotHandler botHandler = BotHandler(client);

// Replace with your network credentials
const char *ssid = "TIM-19861131";
const char *password = "BussolaGay";

WiFiConfiguration wifi(ssid, password);

Controller controller;

void setup()
{
	Serial.begin(115200);
	pinMode(2, OUTPUT);

	configTime(0, 0, "pool.ntp.org"); // get UTC time via NTP
	client.setTrustAnchors(&cert);	  // Add root certificate for api.telegram.org

	controller.begin(&botHandler, &wifi);
}

void loop()
{
	controller.start();
}
