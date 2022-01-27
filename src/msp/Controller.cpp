// #include "Energia.h"
#include "Controller.h"

Controller::Controller()
{
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

Controller::~Controller()
{
}

void Controller::sendState(String chatId) {
    String msgs[5] = {};

    if(this->state == State_AUTOMATIC){
        msgs[0] = "/iThe green house is in automatic mode.\n";
        msgs[1] = "/iNow is " + String((this->myClock->dayCycle == DayCycle_DAY)? "day" : "night") + ".\n";
        msgs[2] = "/iThe time is " + this->myClock->getTimeAsString() + ".\n";
        msgs[3] = "/iThe plant is " + String((this->shouldWatering == true)? "being watered" : "not being watered") + ".\n";
        msgs[4] = "/iSoil sensor is at " + String(this->soilSensor->readSensor()) + "%.\n";
        msgs[5] = "/e" + chatId;
    }else{
        msgs[0] = "/iThe green house is in manual mode.\n";
        msgs[1] = "/iSet parameters.\n";
        msgs[2] = "";
        msgs[3] = "";
        msgs[4] = "";
        msgs[5] = "/e" + chatId;
    }
    
#ifdef DEBUG
        Serial.println("message created.");
#endif

    int i = 0;
    int last_timer = millis();
    while (i < 6) 
    {
        while (millis() - last_timer > 1500)
        {
#ifdef DEBUG
            Serial.print("entered loop: i: " + String(i) + ", msg: ");
            Serial.println(msgs[i]);
#endif
            this->mySerial.send(msgs[i]);

            last_timer = millis();
            i++;
        }
    }

#ifdef DEBUG
        Serial.println("done loop");
#endif
}

void Controller::sendLightManual(String chatId){
    this->mySerial.send("/iThe light is " + String((this->lightManual)? "ON" : "OFF") + ".\n");
    this->mySerial.send("/e" + chatId);
}

void Controller::sendWaterManual(String chatId){
    this->mySerial.send("/iThe plant is " + String((this->waterManual)? "being watered" : "not being watered") + ".\n");
    this->mySerial.send("/e" + chatId);
}

void Controller::sendFanManual(String chatId){
    this->mySerial.send("/iThe fan is " + String((this->fanManual)? "ON" : "OFF") + ".\n");
    this->mySerial.send("/e" + chatId);
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
            // this->state = this->previousState;
            // this->display->clear();
            if (this->previousState == State_AUTOMATIC) {
                this->mySerial.send("/d");
                this->hasSendDate = true;
            } else {
                this->changeState(State_MANUAL);
            }
        }
        break;
    }
}

void Controller::changeState(State state) {
// #ifdef DEBUG
//     Serial.println("WTF: " + String(this->state) + " " + String(state));
// #endif
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
            this->mySerial.send("/sSTART2");
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
            }else{
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
            if (this->hasReceivedState) {
                this->sendState(command.commandText);
                this->hasReceivedState = false;
            }else if(this->hasReceivedLight){
                this->sendLightManual(command.commandText);
                this->hasReceivedLight = false;
            }else if(this->hasReceivedWater){
                this->sendWaterManual(command.commandText);
                this->hasReceivedWater = false;
            }else if(this->hasReceivedFan){
                this->sendFanManual(command.commandText);
                this->hasReceivedFan = false;
            }
            
        }
        default:
            break;
        }

        this->lastCommandRecevied = command;
    }
}

void Controller::home()
{
    int sensorValue = this->soilSensor->readSensor();
    this->shouldWatering = this->soilSensor->shouldWatering(sensorValue);

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