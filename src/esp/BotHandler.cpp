// #include <Arduino.h>
// #include <WiFiClientSecure.h>
// #include <UniversalTelegramBot.h> // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot

#include "BotHandler.h"
// #include "Types.h"

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

void BotHandler::begin(SerialCommunication *softwareSerial, State *state)
{
    this->mySerial = softwareSerial;
    this->mspState = state;

    this->defaultBotCommands = (
        "[" +
        String("{\"command\":\"/start\", \"description\":\"Start bot\"},") +
        String("{\"command\":\"/setautomatic\",\"description\":\"Set mode to automatic\"},") +
        String("{\"command\":\"/setmanual\",\"description\":\"Set mode to manual\"},") +
        String("{\"command\":\"/state\",\"description\":\"Request current state\"},") +
        String("{\"command\":\"/help\",\"description\":\"Get help\"}") +
        "]"
    );

    this->manualBotCommands = (
        "[" +
        String("{\"command\":\"/start\", \"description\":\"Start bot\"},") +
        String("{\"command\":\"/setautomatic\",\"description\":\"Set mode to automatic\"},") +
        String("{\"command\":\"/setmanual\",\"description\":\"Set mode to manual\"},") +
        
        String("{\"command\":\"/changetime\",\"description\":\"Manual - Change time\"},") +
        String("{\"command\":\"/togglelight\",\"description\":\"Manual - Turn on and off light\"},") +
        String("{\"command\":\"/togglewater\",\"description\":\"Manual - Turn on and off water\"},") +
        String("{\"command\":\"/togglefan\",\"description\":\"Manual - Turn on and off fan\"},") +
        
        String("{\"command\":\"/state\",\"description\":\"Request current state\"},") +
        String("{\"command\":\"/help\",\"description\":\"Get help\"}") +
        "]"
    );
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

#ifdef DEBUG
    Serial.println(this->defaultBotCommands);
#endif
    this->bot.setMyCommands(this->defaultBotCommands);
}

void BotHandler::setManual(String chatId)
{
    this->mySerial->send("/cmanual");
    this->sendMessage(chatId, "Mode set to manual.");
    this->sendMessage(chatId, "Set time");

#ifdef DEBUG
    Serial.println(this->manualBotCommands);
#endif
this->bot.setMyCommands(this->manualBotCommands);
}

void BotHandler::state(String chatId)
{
    this->mySerial->send("/cstate");
    this->mySerial->send(String("/i" + chatId));

    this->sendMessage(chatId, String("Gathering information... please wait."));
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

        // if (*this->mspState == State_MANUAL) {
        //     if (text == "/changetime")
        //         this->sendMessage(chatId, "Set time");
        //         // get time from user
        //     if (text == "/togglelight") {
        //         this->mySerial->send("/ctogglelight");
        //         this->mySerial->send(String("/i" + chatId));
        //     }
        //     if (text == "/togglewater") {
        //         this->mySerial->send("/ctogglewater");
        //         this->mySerial->send(String("/i" + chatId));
        //     }
        //     if (text == "/togglefan") {
        //         this->mySerial->send("/ctogglefan");
        //         this->mySerial->send(String("/i" + chatId));
        //     }
        // }
        // } else {
        //     this->sendMessage(chatId, String("You can set the parameters only in manual mode."));
        // }
#ifdef DEBUG
            Serial.println(text);
#endif

        if (text == "/start")
            this->startMessage(chatId, fromName);
        else if (text == "/setautomatic")
            this->setAutomatic(chatId);
        else if (text == "/setmanual")
            this->setManual(chatId);
        else if (text == "/state")
            this->state(chatId);
        else if (text == "/help")
            this->help(chatId);
        else if (text == "/changetime")
            if (*this->mspState == State_MANUAL) {
                this->sendMessage(chatId, "Set time");
            } else {
                this->sendMessage(chatId, "You can send this command only in manual mode.");
            }
        else if (text == "/togglelight") {
            if (*this->mspState == State_MANUAL) {
                this->mySerial->send("/ctogglelight");
                this->mySerial->send(String("/i" + chatId));
            } else {
                this->sendMessage(chatId, "You can send this command only in manual mode.");
            }
        }
        else if (text == "/togglewater") {
            if (*this->mspState == State_MANUAL) {
                this->mySerial->send("/ctogglewater");
                this->mySerial->send(String("/i" + chatId));
            } else {
                this->sendMessage(chatId, "You can send this command only in manual mode.");
            }
        }
        else if (text == "/togglefan") {
            if (*this->mspState == State_MANUAL) {
                this->mySerial->send("/ctogglefan");
                this->mySerial->send(String("/i" + chatId));
            } else {
                this->sendMessage(chatId, "You can send this command only in manual mode.");
            }
        }
        else { // time
            bool isCorrect = false;
            isCorrect = (text.length() <= 5) && isdigit(text.charAt(0)) && isdigit(text.charAt(1)) && (text.charAt(2) == ':') && isdigit(text.charAt(3)) && isdigit(text.charAt(4));

            if (isCorrect)
            {
                if ((text.charAt(0) - '0') <= 1) {
                    isCorrect = isdigit(text.charAt(1)) && (text.charAt(1) - '0') <= 9;
                } else if ((text.charAt(0) - '0') == 2) {
                    isCorrect = isdigit(text.charAt(1)) && (text.charAt(1) - '0') <= 3;
                } else {
                    isCorrect = false;
                }

                isCorrect = (isdigit(text.charAt(3)) && (text.charAt(3) - '0') <= 5) && (isdigit(text.charAt(4)) && (text.charAt(4) - '0') <= 9);
            }
            
            if(isCorrect)
                this->mySerial->send(String("/d" + text));
            else
                this->sendMessage(chatId, "Set time again, the correct format is hh:mm");
        }     
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