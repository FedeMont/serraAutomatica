#ifndef __BOTHANDLER_H__
#define __BOTHANDLER_H__

#include <Arduino.h>
// #include <SoftwareSerial.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h> // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot

#include "SerialCommunication.h"
#include "Types.h"

// #define BOT_TOKEN "5054228318:AAEY4d4M9VQMujE3A-zhw_ao8a5ieW746nU"

class BotHandler
{
private:
    String defaultBotCommands;
    String manualBotCommands;

    String permittedChatIds[2];

    SerialCommunication *mySerial;
    State *mspState;

    const String BOT_TOKEN = "5054228318:AAEY4d4M9VQMujE3A-zhw_ao8a5ieW746nU"; 
    String chat_ids[2] = {"9202122", "658340861"};
public:
    UniversalTelegramBot bot;
    BotHandler(WiFiClientSecure &);
    BotHandler(const String &, WiFiClientSecure &, String[]);
    ~BotHandler();

    unsigned long lastTimeBotRan;
    int botRequestDelay = 1000; // checks for new messages time

    int getUpdates();
    bool isIdPermitted(String);
    void sendMessage(String, String);
    telegramMessage getMessage(int);

    void begin(SerialCommunication *, State *);
    void startMessage(String, String);
    void setAutomatic(String);
    void setManual(String);
    void state(String);
    void help(String);

    void handleNewMessages(int, bool);

    void start(bool);
};

#endif
