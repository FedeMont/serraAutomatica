#include <Arduino.h>
#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>
#include <time.h>

#include "BotHandler.h"
#include "WiFiConfiguration.h"
#include "Types.h"
#include "Controller.h"

#define BOT_TOKEN "5054228318:AAEY4d4M9VQMujE3A-zhw_ao8a5ieW746nU"

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;

// #include <ArduinoJson.h>

String chat_ids[2] = {"9202122", "658340861"};

BotHandler botHandler = BotHandler(BOT_TOKEN, client, chat_ids);

// Replace with your network credentials
const char *ssid = "TIM-19861131";
const char *password = "BussolaGay";

WiFiConfiguration wifi(ssid, password);

Controller controller;

time_t rawtime;

void setup()
{
	Serial.begin(115200);
	pinMode(2, OUTPUT);

	configTime(0, 0, "pool.ntp.org"); // get UTC time via NTP
	client.setTrustAnchors(&cert);	  // Add root certificate for api.telegram.org

	controller.begin(&botHandler, &wifi);

	time(&rawtime);
	struct tm *timeinfo;
	timeinfo = localtime(&rawtime);
	char buffer[80];
	strftime(buffer, 80, "%Y/%m/%dT%r", timeinfo);

#ifdef DEBUG
	Serial.println(buffer);
#endif
}

void loop()
{
	controller.start();
}
