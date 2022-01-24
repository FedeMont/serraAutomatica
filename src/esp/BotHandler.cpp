// #include <Arduino.h>
// #include <WiFiClientSecure.h>
// #include <UniversalTelegramBot.h> // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot

#include "BotHandler.h"
#include "Types.h"

BotHandler::BotHandler(WiFiClientSecure &client) : bot(this->BOT_TOKEN, client)
{
    for (int i = 0; i < (sizeof(this->permittedChatIds) / sizeof(*this->permittedChatIds)); i++)
        this->permittedChatIds[i] = this->chat_ids[i];
}

BotHandler::BotHandler(const String &token, WiFiClientSecure &client, String chatIds[]) : bot(token, client)
{
    for (int i = 0; i < (sizeof(this->permittedChatIds) / sizeof(*this->permittedChatIds)); i++)
        this->permittedChatIds[i] = chatIds[i];
}

BotHandler::~BotHandler()
{
}

int BotHandler::getUpdates()
{
    return this->bot.getUpdates(this->bot.last_message_received + 1);
}

bool BotHandler::isIdPermitted(String id)
{
    bool isIdPermitted = false;

    for (int i = 0; i < (sizeof(this->permittedChatIds) / sizeof(*this->permittedChatIds)); i++)
    {
        if (id == this->permittedChatIds[i])
            isIdPermitted = true;
    }

    return isIdPermitted;
}

void BotHandler::sendMessage(String chatId, String text)
{
    this->bot.sendMessage(chatId, text, "");
}

telegramMessage BotHandler::getMessage(int index)
{
    return this->bot.messages[index];
}

void BotHandler::begin(SerialCommunication *softwareSerial)
{
    this->mySerial = softwareSerial;
}

void BotHandler::startMessage(String chatId, String fromName)
{
    String welcome = "Welcome, " + fromName + ".\n";
    welcome += "Use the following commands to control your greenhouse.\n\n";
    welcome += "/setautomatic to set mode to automatic\n";
    welcome += "/setmanual to set mode to manual\n";
    welcome += "/state to request current mode state\n";
    welcome += "/help to get the command list\n";

    this->sendMessage(chatId, welcome);
}

void BotHandler::setAutomatic(String chatId)
{
    this->mySerial->send("/cautomatic");
    this->sendMessage(chatId, "Mode set to automatic.");
}

void BotHandler::setManual(String chatId)
{
    this->mySerial->send("/cmanual");
    this->sendMessage(chatId, "Mode set to manual.");
}

void BotHandler::state(String chatId)
{
    this->mySerial->send("/cstate::" + chatId);

    // this->sendMessage(chatId, String("State: " + this->mySerial->receive()));
}

void BotHandler::help(String chatId)
{
    String help = "Command list.\n";
    help += "Use the following commands to control your greenhouse.\n\n";
    help += "/setautomatic to set mode to automatic\n";
    help += "/setmanual to set mode to manual\n";
    help += "/state to request current mode state\n";
    help += "/help to get the command list\n";

    this->sendMessage(chatId, help);
}


void BotHandler::handleNewMessages(int newMessages)
{
    for (int i = 0; i < newMessages; i++)
    {
        telegramMessage message = this->getMessage(i);

        String chatId = message.chat_id;
        String text = message.text;
        String fromName = message.from_name;

        if (!this->isIdPermitted(chatId))
        {
            this->sendMessage(chatId, "Unauthorized user");
            continue;
        }

        if (text == "/start")
            this->startMessage(chatId, fromName);
        if (text == "/setautomatic")
            this->setAutomatic(chatId);
        if (text == "/setmanual")
            this->setManual(chatId);
        if (text == "/state")
            this->state(chatId);
        if (text == "/help")
            this->help(chatId);
    }
}

void BotHandler::start() {
    if (millis() > this->lastTimeBotRan + this->botRequestDelay)
    {
        int numNewMessages = this->getUpdates();

        while (numNewMessages)
        {
#ifdef DEBUG
            Serial.println("got response");
#endif
            this->handleNewMessages(numNewMessages);
            numNewMessages = this->getUpdates();
        }

        this->lastTimeBotRan = millis();
    }
}