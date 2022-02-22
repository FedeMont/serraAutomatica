#ifndef __BOTHANDLER_H__
#define __BOTHANDLER_H__

#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h> // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include "FileSystem.h"
#include "Types.h"

#define TELEGRAM_DEBUG

class BotHandler
{
private:
    String dayCycleToString[3] = {"DAY", "NIGHT", "NONSET"};
    String stateToString[3] = {"AUTO", "OFF", "ON"};
    String waterStateToString[3] = {"AUTO", "CLOSED", "OPEN"};

    WiFiClientSecure client;

    FileSystem *fileSystem;

    const String BOT_TOKEN = "5054228318:AAEY4d4M9VQMujE3A-zhw_ao8a5ieW746nU";
    const String BOT_CONNECTION_PSW = "GreenHouseProject2022";
    String logginChatId = "";
    String permittedChatId = "";

    String botCommands;
    String commandsList;

    unsigned long lastTimeBotRan;
    int botRequestDelay = 1000; // checks for new messages time

    State *lightState;
    State *fanState;
    State *waterState;
    long *wateringTimer;
    DayCycle dayCycle;
    float soilSensorPercentage;
    bool shouldWatering;
    String ipAddress;

    int getUpdates();
    telegramMessage getMessage(int);

    bool isIdPermitted(String, bool);
    void handleNewMessages(int);

    void startMessage(String, String);

    void state(String);
    void help(String);

public:
    UniversalTelegramBot bot;

    BotHandler(FileSystem *);
    BotHandler(WiFiClientSecure &);
    BotHandler(const String &, WiFiClientSecure &, String);
    ~BotHandler();

    void begin(String, State *, State *, State *, long *, const String &);

    void setCommands();
    void setCommands(const String &);

    void sendMessage(String, String);

    void setAutomatic(String);

    void start(DayCycle, float, bool);
};

#endif
