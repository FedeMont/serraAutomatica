// #include <Arduino.h>
#include "Controller.h"

Controller::Controller()
{
    this->setDefaultValues();
}

Controller::~Controller()
{
}

void Controller::setDefaultValues() {
    this->hasConnectionTimedOut = false;
    this->isConnectedToMSP = false;

    this->infoMessage = "";
    this->state = State_NONE;
}

void Controller::threeWayHandShake(const String &text) {
    if (text == "START2")
    {
        this->isConnectedToMSP = true;
#ifdef DEBUG
    Serial.println("Connected to MSP");
#endif
        this->mySerial.send("/sSTARTACK");
    }
}

bool Controller::getConnectionState() {
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
}

String Controller::getTime()
{
    this->timeAndDateClient->update();
    return this->timeAndDateClient->getFormattedTime().substring(0, 5);
}

void Controller::readFromMSP(const String &msg) {
    Command command = this->mySerial.commandParser(msg);
    if (command.isValid) {
        switch (command.commandType)
        {
        case 's': { // start
#ifdef DEBUG
            Serial.print("start: ");
            Serial.println(command.commandText);
#endif
            this->threeWayHandShake(command.commandText);
            
        } break;
        case 'd': { // date
#ifdef DEBUG
            Serial.print("date: ");
            Serial.print(command.commandText + ", ");
            Serial.println(this->getTime());
#endif
            this->mySerial.send(String("/d" + this->getTime()));
        } break;
        case 'c': { // command
#ifdef DEBUG
            Serial.print("command: ");
            Serial.println(command.commandText);
#endif
        } break;
        case 'm':{
#ifdef DEBUG
            Serial.print("command: ");
            Serial.println(command.commandText);
#endif
            if(command.commandText == "automatic"){
                this->state = State_AUTOMATIC;
            }else{
                this->state = State_MANUAL;
            }

        }break;
        case 'i': { // info
#ifdef DEBUG
            Serial.print("info: ");
            Serial.println(command.commandText);
            Serial.println(this->infoMessage);
#endif
            this->infoMessage += command.commandText;
        } break;
        case 'e': { //end
#ifdef DEBUG
            Serial.print("end: ");
            Serial.println(command.commandText);
#endif
            this->botHandler->sendMessage(command.commandText, this->infoMessage);
            this->infoMessage = "";
        } break;

        default:
            break;
        }
    }
}

void Controller::start()
{
    this->botHandler->start(!this->hasConnectionTimedOut && this->isConnectedToMSP);
}