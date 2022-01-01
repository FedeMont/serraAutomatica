#include "Controller.h"
// #include <SoftwareSerial.h>

Controller::Controller()
{
}

Controller::~Controller()
{
}

String Controller::getTime()
{
    this->timeAndDateClient->update();
    return this->timeAndDateClient->getFormattedTime().substring(0, 5);
}

void Controller::begin(BotHandler *botHandler, WiFiConfiguration *wiFi, NTPClient *timeClient)
{
    Serial.begin(115200);
    this->mySerial.begin(115200);
    pinMode(2, OUTPUT);

    this->botHandler = botHandler;
    this->wifiConfiguration = wiFi;
    this->timeAndDateClient = timeClient;

    this->botHandler->begin(&this->mySerial);
    this->wifiConfiguration->connect();
    this->timeAndDateClient->begin();
}

void Controller::readFromMSP() {
    if (this->mySerial.isAvailable() && (millis() > this->lastTimeRead + this->readDelay))
    {
        Command command = this->mySerial.receive();
        if (command.isValid) {
            switch (command.commandType)
            {
            case 's': { // start
#ifdef DEBUG
                Serial.print("start: ");
                Serial.println(command.commandText);
#endif
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
            case 'i': { // info
#ifdef DEBUG
                Serial.print("info: ");
                Serial.println(command.commandText);
#endif

            } break;

            default:
                break;
            }
        }
        this->lastTimeRead = millis();
    }
}

void Controller::start()
{
    this->botHandler->start();
}