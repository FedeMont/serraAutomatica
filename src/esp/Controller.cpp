// #include <Arduino.h>
#include "Controller.h"

Controller::Controller()
{
    this->setDefaultValues();
}

Controller::~Controller()
{
}

void Controller::setDefaultValues()
{
    this->hasConnectionTimedOut = false;
    this->isConnectedToMSP = false;

    this->infoMessage = "";
    this->state = State_NONE;
}

void Controller::threeWayHandShake(const String &text)
{
    if (text == "START2")
    {
        this->isConnectedToMSP = true;
#ifdef DEBUG
        Serial.println("Connected to MSP");
#endif
        this->mySerial.send("/sSTARTACK");
    }
}

bool Controller::getConnectionState()
{
    return this->isConnectedToMSP;
}

void Controller::begin(BotHandler *botHandler, WiFiConfiguration *wiFi, NTPClient *timeClient)
{
    Serial.begin(115200);
    this->mySerial.begin(115200);
    pinMode(2, OUTPUT);

    this->botHandler = botHandler;
    this->wifiConfiguration = wiFi;
    this->timeAndDateClient = timeClient;

    this->botHandler->begin(&this->mySerial, &this->state);
    this->wifiConfiguration->connect();
    this->timeAndDateClient->begin();

    this->mySerial.send("/sSTART");
    this->botHandler->setCommands();
}

String Controller::getTime()
{
    this->timeAndDateClient->update();
    return this->timeAndDateClient->getFormattedTime().substring(0, 5);
}


void Controller::stateMsgParser(Command command) 
{
    if (command.commandText == "mode")
    {
        this->infoMessage += "The green house is in " + command.commandValue + " mode.\n\n";
    }
    else if (command.commandText == "time") 
    {
        this->infoMessage += "It's " + command.commandValue + " ";
    }
    else if (command.commandText == "times") 
    {
        this->infoMessage += "Time set to " + command.commandValue + ".";
    }
    else if (command.commandText == "day") 
    {
        this->infoMessage += "and the " + command.commandValue + ", ahem... I mean ";
    }
    else if (command.commandText == "light") 
    {
        this->infoMessage += "the light is " + command.commandValue + ".\n";
    }
    else if (command.commandText == "lights") 
    {
        this->infoMessage += "The light is " + command.commandValue + ".";
    }
    else if (command.commandText == "soil") 
    {
        this->infoMessage += "Soil humdity is at " + command.commandValue + "%, and ";
    }
    else if (command.commandText == "water") 
    {
        this->infoMessage += "the plant is " + command.commandValue + "being watered.\n";
    }
    else if (command.commandText == "waters") 
    {
        this->infoMessage += "The plant is " + command.commandValue + "being watered.";
    }
    else if (command.commandText == "wateron") 
    {
        this->infoMessage += "Water will be open for 3 minutes.";
    }
    else if (command.commandText == "temperature")
    {
        this->infoMessage += "The temperature sensor is measuring " + command.commandValue + "°C, so ";
    }
    else if (command.commandText == "fan") 
    {
        this->infoMessage += "the fan is " + command.commandValue + ".";
    }
    else if (command.commandText == "fan") 
    {
        this->infoMessage += "The fan is " + command.commandValue + ".";
    }
}

void Controller::readFromMSP(const String &msg)
{
    Command command = this->mySerial.commandParser(msg);
    if (command.isValid)
    {
        switch (command.commandType)
        {
        case 's':
        { // start
#ifdef DEBUG
            Serial.print("start: ");
            Serial.println(command.commandText);
#endif
            this->threeWayHandShake(command.commandText);
        }
        break;
        case 'd':
        { // date
#ifdef DEBUG
            Serial.print("date: ");
            Serial.println(command.commandText);
            Serial.println(command.chatId);
            Serial.println(this->getTime());
#endif
            this->mySerial.send(String("/d" + this->getTime()) + "&" + command.chatId);
        }
        break;
        case 'c':
        { // command
#ifdef DEBUG
            Serial.print("command: ");
            Serial.println(command.commandText);
            Serial.println(command.chatId);
#endif
        }
        break;
        case 'm':
        {
#ifdef DEBUG
            Serial.print("command: ");
            Serial.println(command.commandText);
            Serial.println(command.chatId);
#endif
            if (command.commandText == "automatic")
            {
                this->state = State_AUTOMATIC;
                this->botHandler->setAutomatic(command.chatId);
            }
            // else
            // {
            //     this->state = State_MANUAL;
            //     this->botHandler->setManual(command.chatId);
            // }
        }
        break;
        case 'i':
        { // info
#ifdef DEBUG
            Serial.print("info: ");
            Serial.println(command.commandText);
            Serial.println(command.chatId);
            Serial.println(this->infoMessage);
#endif
            this->stateMsgParser(command);
        }
        break;
        case 'e':
        { //end
#ifdef DEBUG
            Serial.print("end: ");
            Serial.println(command.commandText);
            Serial.println(command.chatId);
#endif
            this->botHandler->sendMessage(command.chatId, this->infoMessage);
            this->infoMessage = "";
        }
        break;

        default:
            break;
        }
    }
}

void Controller::start()
{
    this->botHandler->start(!this->hasConnectionTimedOut && this->isConnectedToMSP);
}