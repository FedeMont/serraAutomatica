#ifndef __BOTHANDLER_H__
#define __BOTHANDLER_H__

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h> // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot

#include "Types.h"

class BotHandler
{
private:
    String permittedChatIds[2];
    SoftwareSerial *mySerial;
public:
    UniversalTelegramBot *bot;
    BotHandler(const String &, WiFiClientSecure &, String[]);
    ~BotHandler();

    unsigned long lastTimeBotRan;
    int botRequestDelay = 1000; // checks for new messages time

    int getUpdates();
    bool isIdPermitted(String);
    void sendMessage(String, String);
    telegramMessage getMessage(int);

    void begin(SoftwareSerial*);
    void start(String, String);
    void setAutomatic(String);
    void setManual(String);
    void state(String);
    void help(String);

    void handleNewMessages(int);
};

#endif
