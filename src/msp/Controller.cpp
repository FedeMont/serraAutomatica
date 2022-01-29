#include <Energia.h>
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
    this->isConnectedToESP = false;

    this->state = State_NONE;
    this->previousState = State_NONE;
    this->previousSelectedAction = Action_NONE;

    this->isMinutePassed = true;
    this->shouldWatering = false;
    this->shouldLight = false;
    this->shouldFan = false;

    this->hasSendDate = false;
}

void Controller::threeWayHandShake(const String &text)
{
    if (text == "START")
    {
        this->isConnectedToESP = false;
        this->mySerial.send("/sSTART2");
    }
    else if (text == "STARTACK")
    {
#ifdef DEBUG
        Serial.println("Connected to ESP");
#endif
        this->isConnectedToESP = true;
    }
}

bool Controller::getConnectionState()
{
    return this->isConnectedToESP;
}

void Controller::begin(Display *display, Navigator *navigator, MyClock *myClock, SoilSensor *soilSensor, TemperatureSensor *temperatureSensor)
{
    Serial.begin(115200);
    this->mySerial.begin(115200);

    this->display = display;
    this->navigator = navigator;
    this->myClock = myClock;
    this->soilSensor = soilSensor;
    this->temperatureSensor = temperatureSensor;

    this->display->begin();
    this->navigator->begin();
    this->temperatureSensor->begin();
}

void Controller::wait()
{
    long timer = millis();
    while (millis() - timer < 500);
}

void Controller::sendEndMessage(const String &chatId)
{
    this->mySerial.send("/eend&" + chatId);
}

void Controller::sendState(const String &chatId)
{
    this->sendModeState();
    this->wait();

    this->sendTimeState("");
    this->wait();
    this->sendDayCycleState();
    this->wait();
    this->sendLightState("");
    this->wait();

    this->sendSoilState();
    this->wait();
    this->sendWateringState("");
    this->wait();

    this->sendTemperatureState();
    this->wait();
    this->sendFanState("");
    this->wait();
    
    this->sendEndMessage(chatId);
}

void Controller::sendModeState()
{
    this->mySerial.send("/imode=" + String((this->state == State_AUTOMATIC) ? "automatic" : "manual"));
}

void Controller::sendTimeState(const String &chatId)
{
    if (chatId == "")
    {
        this->mySerial.send("/itime=" + this->myClock->getTimeAsString());
    }
    else
    {
        this->mySerial.send("/itimes=" + this->myClock->getTimeAsString());
        this->wait();
        this->sendEndMessage(chatId);
    }
}

void Controller::sendDayCycleState()
{
    this->mySerial.send("/iday=" + String((this->shouldLight) ? "sun is shining" : "moon is up"));
}

void Controller::sendLightState(const String &chatId)
{
    if (chatId != "")
    {
        this->mySerial.send("/ilights=" + String((this->shouldLight) ? "on" : "off"));
        this->wait();
        this->sendEndMessage(chatId);
    }else{
        this->mySerial.send("/ilight=" + String((this->shouldLight) ? "on" : "off"));

    }
}

void Controller::sendSoilState()
{
    this->mySerial.send("/isoil=" + String(this->soilSensor->valueToPercentage(this->soilSensor->readSensor())));
}

void Controller::sendWateringState(const String &chatId)
{
    if (chatId != "")
    {
        this->mySerial.send("/iwaters=" + String((this->shouldWatering == false) ? "not ": ""));
        this->wait();
        this->sendEndMessage(chatId);
    }else{
        this->mySerial.send("/iwater=" + String((this->shouldWatering == false) ? "not " : ""));
    }
}

void Controller::sendTemperatureState()
{
    this->mySerial.send("/itemperature=" + String(this->temperatureSensor->readDiet()));
}

void Controller::sendFanState(const String &chatId)
{
    if (chatId != "")
    {
        this->mySerial.send("/ifans=" + String((this->shouldFan) ? "spinning" : "off"));
        this->wait();
        this->sendEndMessage(chatId);
    }else{
        this->mySerial.send("/ifan=" + String((this->shouldFan) ? "spinning" : "off"));
    }
}

void Controller::manualChooseState(Action action)
{
    switch (action)
    {
    default:
#ifdef DEBUG
        Serial.print("Action: ");
        Serial.println(int(action));
        Serial.print("Previous action: ");
        Serial.println(int(this->previousSelectedAction));
        Serial.print("State: ");
        Serial.println(int(this->state));
#endif
    case Action_NONE:
        switch (this->previousSelectedAction)
        {
        case Action_NONE:
            this->previousSelectedAction = Action_UP;
            this->previousState = State_MANUAL;
            this->display->chooseState(this->previousState);
            ;
            break;
        default:
            break;
        }
        break;
    case Action_UP:
        this->previousSelectedAction = Action_UP;
        this->previousState = State_MANUAL;
        this->display->chooseState(this->previousState);
        break;
    case Action_DOWN:
        this->previousSelectedAction = Action_DOWN;
        this->previousState = State_AUTOMATIC;
        this->display->chooseState(this->previousState);
        break;
    case Action_SELECT:
        if (this->previousSelectedAction != Action_NONE)
        {
            if (this->previousState == State_AUTOMATIC)
            {
                this->mySerial.send("/d");
                this->hasSendDate = true;
            }
            else
            {
                this->changeState(State_MANUAL);
            }
        }
        break;
    }
}

void Controller::changeState(State state)
{
    if (this->state != state)
    {
        this->isMinutePassed = true;

        this->hasSendDate = false;

        this->previousState = State_NONE;
        this->previousSelectedAction = Action_NONE;

        this->state = state;
        this->display->resetHomeScreenFlags();
        this->display->clear();
    }
}

void Controller::chooseTime(Action action)
{
    this->display->chooseTime(this->myClock->selectedDigit, this->myClock->digits);
    this->myClock->chooseTime(action);
}

void Controller::readFromESP(const String &msg)
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
            Serial.println(this->hasSendDate);
#endif
            this->myClock->saveTime(command.commandText);
            if (this->hasSendDate)
            {
                this->changeState(State_AUTOMATIC);
                this->mySerial.send("/mautomatic&" + command.chatId);
                this->hasSendDate = false;
            }
            else
            {
                this->isMinutePassed = true;
                this->display->resetHomeScreenFlags();
                this->display->clear();
                this->sendTimeState(command.chatId);
            }
        }
        break;
        case 'c':
        { // command
#ifdef DEBUG
            Serial.print("command: ");
            Serial.println(command.commandText);
            Serial.println(command.chatId);
#endif
            if (command.commandText == "automatic")
            {
                this->mySerial.send("/ddate&" + command.chatId);
                this->hasSendDate = true;
            }
            else if (command.commandText == "manual")
            {
                this->changeState(State_MANUAL);
                this->mySerial.send("/mmanual&" + command.chatId);
                this->myClock->isTimeSaved = false;
            }
            else if (command.commandText == "state")
            {
                this->sendState(command.chatId);
            }
            else if (command.commandText == "togglelight")
            {
                if (!this->shouldLight)
                {
                    this->navigator->lightOn();
#ifdef DEBUG
                    Serial.println("Light on");
#endif
                }
                else
                {
                    this->navigator->lightOff();
#ifdef DEBUG
                    Serial.println("Light off");
#endif
                }
                this->shouldLight = !this->shouldLight;
                this->sendLightState(command.chatId);
            }
            else if (command.commandText == "togglewater")
            {
                if (!this->shouldWatering)
                {
                    this->navigator->waterOn();
                    this->mySerial.send("/iwateron&" + command.chatId);
                    this->wateringTimer = millis();
#ifdef DEBUG
                    Serial.println("Water on");
#endif
                }
                else
                {
                    this->navigator->waterOff();
#ifdef DEBUG
                    Serial.println("Water off");
#endif
                }
                this->shouldWatering = !this->shouldWatering;
                this->sendWateringState(command.chatId);
            }
            else if (command.commandText == "togglefan")
            {
                if (!this->shouldFan)
                {
                    this->navigator->fanOn();
#ifdef DEBUG
                    Serial.println("Fan on");
#endif
                }
                else
                {
                    this->navigator->fanOff();
#ifdef DEBUG
                    Serial.println("Fan off");
#endif
                }
                this->shouldFan = !this->shouldFan;
                this->sendFanState(command.chatId);
            }
        }
        break;
        case 'i':
        {
#ifdef DEBUG
            Serial.print("info: ");
            Serial.println(command.commandText);
            Serial.println(command.chatId);
#endif
        }
        default:
            break;
        }
    }
}

void Controller::automaticStart()
{
    int soilSensorValue = this->soilSensor->readSensor();
    float temperatureSensorValue = this->temperatureSensor->readDiet();

    this->shouldWatering = this->soilSensor->shouldWatering(soilSensorValue);
    float soilSensorPercentage = this->soilSensor->valueToPercentage(soilSensorValue);

    this->display->homeScreen(this->myClock->getTimeAsString(), this->isMinutePassed, this->myClock->dayCycle, soilSensorPercentage, temperatureSensorValue, this->shouldWatering); // first write

    if (this->shouldWatering)
    {
        this->navigator->waterOn();
    }
    else
    {
        this->navigator->waterOff();
    }

    this->isMinutePassed = this->myClock->isMinutePassed(); // check if minute is really passed
    this->myClock->clock(this->isMinutePassed);

    this->shouldLight = (this->myClock->dayCycle == DayCycle_DAY);

    if (this->shouldLight)
    {
        this->navigator->lightOn();
    }
    else
    {
        this->navigator->lightOff();
    }

    this->shouldFan = this->temperatureSensor->shouldFan(temperatureSensorValue);

    if (this->shouldFan)
    {
        this->navigator->fanOn();
    }
    else
    {
        this->navigator->fanOff();
    }
}

void Controller::manualStart(Action action)
{
    if (!this->myClock->isTimeSaved)
    {
        if (action == Action_SELECT)
        {
            this->myClock->saveTime();
            this->display->clear();
        }
        else
        {
            this->chooseTime(action);
        }
    }
    else
    {
        int soilSensorValue = this->soilSensor->readSensor();
        float temperatureSensorValue = this->temperatureSensor->readDiet();
        float soilSensorPercentage = this->soilSensor->valueToPercentage(soilSensorValue);

        this->display->homeScreen(this->myClock->getTimeAsString(), this->isMinutePassed, (this->shouldLight)? DayCycle_DAY : DayCycle_NIGHT, soilSensorPercentage, temperatureSensorValue, shouldWatering); // first write

        this->isMinutePassed = this->myClock->isMinutePassed(); // check if minute is really passed
        this->myClock->clock(this->isMinutePassed);

        if (this->shouldWatering) {
            if (millis() - this->wateringTimer > 180000) { // 3 minutes = 180000
                this->shouldWatering = false;
                this->navigator->waterOff();
#ifdef DEBUG
                Serial.println("WATER OFF CAUSE OF TIMER");
#endif
            }
        }
    }
}

void Controller::connectionTimeOut()
{
    this->hasConnectionTimedOut = true;
    this->display->clear();
}

void Controller::start()
{
    Action action = this->navigator->getAction();

    if (!this->hasConnectionTimedOut)
    {
        switch (this->state)
        {
        case State_NONE:
            this->manualChooseState(action);
            break;
        case State_MANUAL:
            this->manualStart(action);
            break;
        case State_AUTOMATIC:
            this->automaticStart();
            break;
        default:
            break;
        }
    }
    else
    {
        this->manualStart(action);
    }

    serialEventRun1();
}