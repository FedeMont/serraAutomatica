// #include <Energia.h>
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
    this->isConnectedToESP = false;

    this->state = State_NONE;
    this->previousState = State_NONE;
    this->previousSelectedAction = Action_NONE;

    this->isMinutePassed = true;
    this->shouldWatering = false;

    this->hasReceivedState = false;
    this->hasReceivedLight = false;
    this->hasReceivedWater = false;
    this->hasReceivedFan = false;
    this->hasSendDate = false;

    this->lightManual = false;
    this->waterManual = false;
    this->fanManual = false;
}

void Controller::threeWayHandShake(const String &text) {
    if (text == "START")
    {
        this->isConnectedToESP = false;
        this->mySerial.send("/sSTART2");
    }
    else if (text == "STARTACK") {
#ifdef DEBUG
    Serial.println("Connected to ESP");
#endif
        this->isConnectedToESP = true;
    }
}

bool Controller::getConnectionState() {
    return this->isConnectedToESP;
}

void Controller::begin(Display *display, Navigator *navigator, MyClock *myClock, SoilSensor *soilSensor)
{
    Serial.begin(115200);
    this->mySerial.begin(115200);

    this->display = display;
    this->navigator = navigator;
    this->myClock = myClock;
    this->soilSensor = soilSensor;

    this->display->begin();
    this->navigator->begin();
}

void Controller::wait() {
    logn timer = millis();
    while (millis() - timer < 1500);
}

void Controller::sendEndMessage(const String &chatId) {
    this->myserial.send("/e" + chatId);
}

void Controller::sendState(String chatId) 
{
    this->sendModeState();
    this->wait();

    if (this->state == State_AUTOMATIC) 
    {
        this->sendDayCycleState();
        this->wait();
        this->sendTimeState();
        this->wait();
        this->sendSoilState();
        this->wait();
        this->sendWateringState();
    }
    else 
    {

    }

    this->wait();
    this->sendEndMessage(chatId);
}

void Controller::sendModeState()
{
    if (this->state == State_AUTOMATIC) 
    {
        this->mySerial.send("/iThe green house is in automatic mode.\n");
    } else 
    {
        this->mySerial.send("/iThe green house is in manual mode.\n");
    }
}

void Controller::sendDayCycleState()
{
    this->mySerial.send("/iNow is " + String((this->myClock->dayCycle == DayCycle_DAY)? "day" : "night") + ".\n");
}

void Controller::sendTimeState()
{
    this->mySerial.send("/iThe time is " + this->myClock->getTimeAsString() + ".\n");
}

void Controller::sendWateringState()
{
    this->mySerial.send("/iThe plant is " + String((this->shouldWatering == true)? "being watered" : "not being watered") + ".\n");
}

void Controller::sendSoilState()
{
    this->mySerial.send("/iSoil sensor is at " + String(this->soilSensor->readSensor()) + "%.\n");
}

void Controller::sendLightManual(const String &chatId) 
{
    this->mySerial.send("/iThe light is " + String((this->lightManual)? "ON" : "OFF") + ".\n");
    this->wait();
    this->sendEndMessage(chatId);

    this->hasReceivedLight = false;
}

void Controller::sendWaterManual(const String &chatId)
{
    this->mySerial.send("/iThe plant is " + String((this->waterManual)? "being watered" : "not being watered") + ".\n");
    this->wait();
    this->sendEndMessage(chatId);

    this->hasReceivedWater = false;
}

void Controller::sendFanManual(const String &chatId)
{
    this->mySerial.send("/iThe fan is " + String((this->fanManual)? "ON" : "OFF") + ".\n");
    this->wait();
    this->sendEndMessage(chatId);

    this->hasReceivedFan = false;
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
            this->display->chooseState(this->previousState);;
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
            if (this->previousState == State_AUTOMATIC) {
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

void Controller::changeState(State state) {
    if (this->state != state) {

        this->isMinutePassed = true;

        this->hasReceivedState = false;
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
            Serial.println(command.commandText);
            Serial.println(this->hasSendDate);
#endif
            this->myClock->saveTime(command.commandText);
            if (this->hasSendDate) {
                this->changeState(State_AUTOMATIC);
                this->hasSendDate = false;
            } else {
                this->isMinutePassed = true;
                this->display->resetHomeScreenFlags();
                this->display->clear();
            }
        } break;
        case 'c': { // command
#ifdef DEBUG
            Serial.print("command: ");
            Serial.println(command.commandText);
#endif
            if (command.commandText == "automatic") {
                this->mySerial.send("/mautomatic");
                this->mySerial.send("/d");
                this->hasSendDate = true;

            } else if (command.commandText == "manual") {
                this->mySerial.send("/mmanual");
                this->changeState(State_MANUAL);
                this->myClock->isTimeSaved = false;

            } else if (command.commandText == "state") {
                this->hasReceivedState = true;
                // Command chatId_command = this->mySerial.rehasReceivedLight
            } else if (command.commandText == "togglelight"){
                this->hasReceivedLight = true;
                if(!this->lightManual) {
                    this->navigator->lightOn();
#ifdef DEBUG
                    Serial.println("Light on");
#endif
                }else {
                    this->navigator->lightOff();
#ifdef DEBUG
                    Serial.println("Light off");
#endif
                }
                this->lightManual = !this->lightManual;

            }else if (command.commandText == "togglewater"){
                this->hasReceivedWater = true;
                if(!this->waterManual) {
                    this->navigator->waterOn();
#ifdef DEBUG
                    Serial.println("Water on");
#endif
                }else {
                    this->navigator->waterOff();
#ifdef DEBUG
                    Serial.println("Water off");
#endif
                }
                this->waterManual = !this->waterManual;

            } else if (command.commandText == "togglefan"){
                this->hasReceivedFan = true;
                if(!this->fanManual) {
                    this->navigator->fanOn();
#ifdef DEBUG
                    Serial.println("Fan on");
#endif
                }
                else {
                    this->navigator->fanOff();
#ifdef DEBUG
                    Serial.println("Fan off");
#endif
                }
                this->fanManual = !this->fanManual;
            }

        } break;
        case 'i': {
#ifdef DEBUG
            Serial.print("info: ");
            Serial.println(command.commandText);
#endif
            if (this->hasReceivedState)
            {
                this->sendState(command.commandText);
                this->hasReceivedState = false;
            }
            else if (this->hasReceivedLight)
            {
                this->sendLightManual(command.commandText);
            }
            else if(this->hasReceivedWater)
            {
                this->sendWaterManual(command.commandText);
            }
            else if(this->hasReceivedFan)
            {
                this->sendFanManual(command.commandText);
            }
        }
        default:
            break;
        }
    }
}

void Controller::automaticStart()
{
    int sensorValue = this->soilSensor->readSensor();
    this->shouldWatering = this->soilSensor->shouldWatering(sensorValue);

    this->display->homeScreen(this->myClock->getTimeAsString(), this->isMinutePassed, this->myClock->dayCycle, sensorValue, this->shouldWatering); // first write

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

    if (this->myClock->dayCycle == DayCycle_DAY)
    {
        this->navigator->lightOn();
    }
    else
    {
        this->navigator->lightOff();
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
        int sensorValue = this->soilSensor->readSensor();
        this->display->homeScreen(this->myClock->getTimeAsString(), this->isMinutePassed, this->myClock->dayCycle, sensorValue, shouldWatering); // first write
        this->isMinutePassed = this->myClock->isMinutePassed(); // check if minute is really passed
        this->myClock->clock(this->isMinutePassed);
    }
}

// void Controller::automaticStart()
// {
//     this->home();
// }

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
    // writeToESP();
}