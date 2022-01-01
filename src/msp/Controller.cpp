// #include "Energia.h"
#include "Controller.h"

Controller::Controller()
{
}

Controller::~Controller()
{
}

String Controller::stateToString() {
    return (this->state == State::MANUAL)? "manual" : ((this->state == State::AUTOMATIC)? "automatic" : "none");
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

void Controller::chooseState(Action action)
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
    case Action::NONE:
        switch (this->previousSelectedAction)
        {
        case Action::NONE:
            this->previousSelectedAction = Action::UP;
            this->previousState = State::MANUAL;
            this->display->chooseState(this->previousState);;
            break;
        default:
            break;
        }
        break;
    case Action::UP:
        this->previousSelectedAction = Action::UP;
        this->previousState = State::MANUAL;
        this->display->chooseState(this->previousState);
        break;
    case Action::DOWN:
        this->previousSelectedAction = Action::DOWN;
        this->previousState = State::AUTOMATIC;
        this->display->chooseState(this->previousState);
        break;
    case Action::SELECT:
        if (this->previousSelectedAction != Action::NONE)
        {
            this->state = this->previousState;
            this->display->clear();
        }
        break;
    }
}

void Controller::changeState(State state) {
    if (this->state != state) {
        this->display->clear();
        this->state = state;
    }
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
                Serial.println(command.commandText);
#endif
                this->myClock->saveTime(command.commandText);
                this->changeState(State::AUTOMATIC);

            } break;
            case 'c': { // command
#ifdef DEBUG
                Serial.print("command: ");
                Serial.println(command.commandText);
#endif
                if (command.commandText == "automatic") {
                    this->mySerial.send("/d");

                } else if (command.commandText == "manual") {
                    this->changeState(State::MANUAL);

                } else if (command.commandText == "state") {
                    this->mySerial.send(String("/i" + this->stateToString()));

                }

            } break;
            default:
                break;
            }

            this->lastCommandRecevied = command;
        }
        
        this->lastTimeRead = millis();
    }
}

void Controller::home()
{
    int sensorValue = this->soilSensor->readSensor();
    bool shouldWatering = this->soilSensor->shouldWatering(sensorValue);

    this->display->homeScreen(this->myClock->getTimeAsString(), this->isMinutePassed, this->myClock->dayCycle, sensorValue, shouldWatering); // first write

    if (shouldWatering)
    {
        this->navigator->waterOn();
    }
    else
    {
        this->navigator->waterOff();
    }

    this->isMinutePassed = this->myClock->isMinutePassed(); // check if minute is really passed
    this->myClock->clock(this->isMinutePassed);

    if (this->myClock->dayCycle == DayCycle::DAY)
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
        if (action == Action::SELECT)
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
        this->home();
    }
}

void Controller::automaticStart()
{
    this->home();
}

void Controller::start()
{
    Action action = this->navigator->getAction();

    switch (this->state)
    {
    case State::NONE:
        this->chooseState(action);
        break;
    case State::MANUAL:
        this->manualStart(action);
        break;
    case State::AUTOMATIC:
        this->automaticStart();
        break;
    default:
        break;
    }

    serialEventRun1();
    // writeToESP();
}