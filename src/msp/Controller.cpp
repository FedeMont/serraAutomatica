// #include "Energia.h"
#include "Controller.h"

Controller::Controller()
{
    this->state = State_NONE;
    this->previousState = State_NONE;
    this->previousSelectedAction = Action_NONE;

    this->isMinutePassed = true;

    this->shouldWatering = false;
    this->shouldLight = false;
    this->shouldFan = false;

    this->hasReceivedState = false;
    this->hasReceivedLight = false;
    this->hasReceivedWater = false;
    this->hasReceivedFan = false;
    this->hasSendDate = false;
}

Controller::~Controller()
{
}

void Controller::wait(){
    int last_timer = millis();
    while (millis() - last_timer < 1500);
}

void Controller::sendState(String chatId) {
    this->sendMode();
    this->wait();

    this->sendTime();
    this->wait();
    this->sendDayCycle();
    this->wait();

    this->sendSoilSensor();
    this->wait();
    this->sendTemperatureSensor();
    this->wait();

    this->sendWater("");
    this->wait();
    
    this->sendLight("");
    this->wait();
    this->sendFan("");
    this->wait();

    this->endMsg(chatId);
}

void Controller::sendMode(){
    this->mySerial.send("/iThe green house is in " + String((this->state == State_AUTOMATIC)? "automatic" : "manual") + " mode.\n\n");
}

void Controller::sendTime(){
    this->mySerial.send("/iIt's " + this->myClock->getTimeAsString());
}

void Controller::sendDayCycle(){
    this->mySerial.send("/i and " + String((this->myClock->dayCycle == DayCycle_DAY)? "sun is shining" : "moon is up") + ".\n");
}

void Controller::sendSoilSensor(){
    this->mySerial.send("/iSoil sensor is at " + String(this->soilSensor->valueToPercentage(this->soilSensor->readSensor())) + "%, and");
}

void Controller::sendTemperatureSensor(){
    this->mySerial.send("/i temperature sensor measure " + String(this->temperatureSensor->readDiet()) + "°C.\n");
}

void Controller::sendWater(String chatId){
    this->mySerial.send("/iThe plant is " + String((this->shouldWatering)? "being watered" : "not being watered") + ".\n");
    if(chatId != ""){
        wait();
        this->endMsg(chatId);
    }
}

void Controller::sendLight(String chatId){
    if(chatId != ""){
        this->mySerial.send("/iThe light is " + String((this->shouldLight)? "on" : "off") + ".\n");
        wait();
        this->endMsg(chatId);
    }else{
        this->mySerial.send("/iThe light is " + String((this->shouldLight)? "on" : "off") + " and");
    }
}

void Controller::sendFan(String chatId){
    
    if(chatId != ""){
        this->mySerial.send("/iThe fan is " + String((this->shouldFan)? "on" : "off") + ".\n");
        wait();
        this->endMsg(chatId);
    }else{
        String too = (this->shouldFan == this->shouldLight)? " too" : "";
        this->mySerial.send("/i the fan is " + String((this->shouldFan)? "on" : "off") + too + ".\n");
    }
}

void Controller::endMsg(String chatId){
    this->mySerial.send("/e" + chatId);
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
#ifdef DEBUG
                Serial.println("State");
#endif

            } else if (command.commandText == "togglelight"){
                this->hasReceivedLight = true;
                if(!this->shouldLight) {
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
                this->shouldLight = !this->shouldLight;

            }else if (command.commandText == "togglewater"){
                this->hasReceivedWater = true;
                if(!this->shouldWatering) {
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
                this->shouldWatering = !this->shouldWatering;

            } else if (command.commandText == "togglefan"){
                this->hasReceivedFan = true;
                if(!this->shouldFan) {
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
                this->shouldFan = !this->shouldFan;
            }

        } break;
        case 'i': { // info
#ifdef DEBUG
            Serial.print("info: ");
            Serial.println(command.commandText);
#endif
            if (this->hasReceivedState) {
                this->sendState(command.commandText);
                this->hasReceivedState = false;
            }else if(this->hasReceivedLight){
                this->sendLight(command.commandText);
                this->hasReceivedLight = false;
            }else if(this->hasReceivedWater){
                this->sendWater(command.commandText);
                this->hasReceivedWater = false;
            }else if(this->hasReceivedFan){
                this->sendFan(command.commandText);
                this->hasReceivedFan = false;
            } 
            
        }
        default:
            break;
        }

        this->lastCommandRecevied = command;
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

    if (this->myClock->dayCycle == DayCycle_DAY)
    {
        this->shouldLight = true;
        this->navigator->lightOn();
    }
    else
    {
        this->shouldLight = false;
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
        
        this->display->homeScreen(this->myClock->getTimeAsString(), this->isMinutePassed, (this->shouldLight)? DayCycle_DAY : DayCycle_NIGHT, soilSensorPercentage, temperatureSensorValue, this->shouldWatering);

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
    
// #ifdef DEBUG
//     Serial.print("Object Temperature: "); 
// #endif
    // Serial.println(this->temperatureSensor->readObject());
    
// #ifdef DEBUG
//     Serial.print("Die Temperature: "); 
// #endif
    // Serial.println(this->temperatureSensor->readDiet());
    // writeToESP();
}