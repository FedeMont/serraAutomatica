#ifndef __BOTHANDLER_H__
#define __BOTHANDLER_H__

#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h> // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot

#include "SerialCommunication.h"
#include "Types.h"

// #define BOT_TOKEN "5054228318:AAEY4d4M9VQMujE3A-zhw_ao8a5ieW746nU"

class BotHandler
{
private:
    const String BOT_TOKEN = "5054228318:AAEY4d4M9VQMujE3A-zhw_ao8a5ieW746nU";
    const String BOT_CONNECTION_PSW = "GreenHouseProject2022";
    // String chat_ids[2] = {"9202122", "658340861"};
    String logginChatId = "";
    String permittedChatId = "";

    String botCommands;
    String commandsList;

    SerialCommunication *mySerial;
    State *mspState;

    unsigned long lastTimeBotRan;
    int botRequestDelay = 1000; // checks for new messages time

    bool shouldSetTime = false;

    bool isIdPermitted(String, bool);

    int getUpdates();
    void handleNewMessages(int, bool);
    telegramMessage getMessage(int);

    void startMessage(String, String);

    void state(String);
    void help(String);

public:
    UniversalTelegramBot bot;

    BotHandler(WiFiClientSecure &);
    BotHandler(const String &, WiFiClientSecure &, String);
    ~BotHandler();

    void setCommands();
    void setCommands(const String &);

    void sendMessage(String, String);

    void begin(SerialCommunication *, State *);
    void setAutomatic(String);
    void setManual(String);

    void start(bool);
};

#endif
