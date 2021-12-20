// #include "Energia.h"
#include "Controller.h"

Controller::Controller()
{
}

Controller::~Controller()
{
}

void Controller::begin(Display *display, Navigator navigator, MyClock *myClock, SoilSensor *soilSensor)
{
    this->display = display;
    this->navigator = navigator;
    this->myClock = myClock;
    this->soilSensor = soilSensor;
    
    Serial1.begin(115200);
}

void Controller::chooseTime(Action action)
{
    this->display->chooseTime(this->myClock->selectedDigit, this->myClock->digits);
    this->myClock->chooseTime(action);
}


void Controller::readFromESP()
{
    if (millis() > this->lastTimeRead + this->readDelay)
    {

        String command = Serial1.readStringUntil('\n');

        this->lastCommandRecevied.commandType = command.substring(0, 2);
        this->lastCommandRecevied.commandText = command.substring(2);

        if (this->lastCommandRecevied.commandType == "/s")
        {
#ifdef DEBUG
            Serial.print("start: ");
            Serial.println(this->lastCommandRecevied.commandText);
#endif
        }
        else if (this->lastCommandRecevied.commandType == "/d")
        {
#ifdef DEBUG
            Serial.print("date: ");
            Serial.println(this->lastCommandRecevied.commandText);
#endif
        }
        else if (this->lastCommandRecevied.commandType == "/c")
        {
#ifdef DEBUG
            Serial.print("command: ");
            Serial.println(this->lastCommandRecevied.commandText);
#endif
            
        }

#ifdef DEBUG
        Serial.println(command);
#endif
        this->lastTimeRead = millis();
    }
}

void Controller::writeToESP()
{
    if (this->lastCommandRecevied.commandType == "/c")
    {
        // if (this->lastCommandRecevied.commandText == "automatic")
        // {
        // }
        // else if (this->lastCommandRecevied.commandText == "manual")
        // {
        // }
        // else
        if (this->lastCommandRecevied.commandText == "state")
        {
            switch (this->state)
            {
            case State::MANUAL:
                Serial1.print("manual\n");
                break;
            case State::AUTOMATIC:
                Serial1.print("automatic\n");
                break;

            default:
                break;
            }
        }
    }
}

void Controller::manualStart()
{
    this->action = this->navigator.getAction();

    if (!this->myClock->isTimeSaved)
    {
        if (this->action == SELECT)
        {
            this->myClock->saveTime();
            this->display->clear();
        }
        else
        {
            this->chooseTime(this->action);
        }
    }
    else
    {
        int sensorValue = this->soilSensor->readSensor();
        bool shouldWatering = this->soilSensor->shouldWatering(sensorValue);

        this->display->homeScreen(this->myClock->getTimeAsString(), this->isMinutePassed, this->myClock->dayCycle, sensorValue, shouldWatering); // first write

        if (shouldWatering)
        {
            this->navigator.waterOn();
        }
        else
        {
            this->navigator.waterOff();
        }

        this->isMinutePassed = this->myClock->isMinutePassed(); // check if minute is really passed
        this->myClock->clock(this->isMinutePassed);

        if (this->myClock->dayCycle == DAY)
        {
            this->navigator.lightOn();
        }
        else
        {
            this->navigator.lightOff();
        }
    }
}

void Controller::automaticStart()
{
}

void Controller::start()
{
    if (this->state == MANUAL)
    {
        this->manualStart();
    }
    else
    {
        this->automaticStart();
    }

    serialEventRun1();
    writeToESP();
}