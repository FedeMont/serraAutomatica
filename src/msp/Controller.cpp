// #include "Energia.h"
#include "Controller.h"

Controller::Controller()
{
    this->state = State_NONE;
    this->previousState = State_NONE;
    this->previousSelectedAction = Action_NONE;

    this->readDelay = 100;

    this->isMinutePassed = true;
    this->isWatering = false;
}

Controller::~Controller()
{
}

void Controller::sendState(String chatId) {

    String msgs[5] = {};

    msgs[0] = "/iThe green house is in " + String((this->state == State_MANUAL)? "manual" :"automatic") + " mode.\n";
    msgs[1] = "/iNow is " + String((this->myClock->dayCycle == DayCycle_DAY)? "day" : "night") + ".\n";
    msgs[2] = "/iThe time is " + this->myClock->getTimeAsString() + ".\n";
    msgs[3] = "/iThe plant is " + String((this->isWatering == true)? "being watered" : "not being watered") + ".\n";
    msgs[4] = "/iSoil sensor is at " + String(this->soilSensor->readSensor()) + "%.\n";
    msgs[5] = "/e" + chatId;

#ifdef DEBUG
        Serial.println("message created.");
#endif

    // this->mySerial.send(msg);
    int i = 0;
    int last_timer = millis();
    while (millis() > last_timer + 5 && i < 6) {
#ifdef DEBUG
        Serial.print("entered loop: i: " + String(i) + ", msg: ");
        Serial.println(msgs[i]);
#endif
        this->mySerial.send(msgs[i]);

        last_timer = millis();
        i++;
    }

#ifdef DEBUG
        Serial.println("done loop");
#endif
    

    // this->mySerial.send(String("/iThe green house is in " + String((this->state == State_MANUAL)? "manual" :"automatic") + " mode."));
    // this->mySerial.send(String("/iNow is " + String((this->myClock->dayCycle == DayCycle_DAY)? "day" : "night") + "."));
    // this->mySerial.send(String("/iThe time is " + this->myClock->getTimeAsString() + "."));
    // this->mySerial.send(String("/iThe plant is " + String((this->isWatering == true)? "being watered" : "not being watered") + "."));
    // this->mySerial.send(String("/iSoil sensor is at " + String(this->soilSensor->readSensor()) + "%."));

    // this->mySerial.send(String("/e" + chatId));
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
            this->state = this->previousState;
            this->display->clear();
            if (this->state == State_AUTOMATIC)
                this->mySerial.send("/d");
                this->readFromESP();
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
                this->mySerial.send("/sSTART2");
            } break;
            case 'd': { // date
#ifdef DEBUG
                Serial.print("date: ");
                Serial.println(command.commandText);
#endif
                this->myClock->saveTime(command.commandText);
                this->changeState(State_AUTOMATIC);

            } break;
            case 'c': { // command
#ifdef DEBUG
                Serial.print("command: ");
                Serial.println(command.commandText);
#endif
                if (command.commandText == "automatic") {
                    this->mySerial.send("/d");

                } else if (command.commandText == "manual") {
                    this->changeState(State_MANUAL);

                } else if (command.commandText == "state") {
                    Command chatId_command = this->mySerial.receive();
                    this->sendState(chatId_command.commandText);
                }

            } break;
//             case 'i': {
// #ifdef DEBUG
//                 Serial.print("info: ");
//                 Serial.println(command.commandText);
// #endif
//                 if (command.commandText == "state") {
//                     this->sendState(command.chatId);
//                 }
//             }
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
        this->isWatering = true;
        this->navigator->waterOn();
    }
    else
    {
        this->isWatering = false;
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
    case State_NONE:
        this->chooseState(action);
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

    serialEventRun1();
    // writeToESP();
}