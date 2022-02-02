#include <Energia.h>
#include "Controller.h"

Controller::Controller()
{
    this->firstTimePowerOn = true;

    this->hasConnectionTimedOut = false;
    this->isConnectedToESP = false;

    this->isMinutePassed = true;
    this->shouldWatering = false;
    this->shouldLight = false;
    this->shouldFan = false;

    this->isLightAuto = true;
    this->isFanAuto = true;
    this->isWaterAuto = true;

    this->waterCounter = 0;
    this->lightCounter = 0;
    this->fanCounter = 0;

    this->hasSentDate = false;
}

Controller::~Controller()
{
}

// public
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
            Serial.println(this->hasSentDate);
#endif
            this->myClock->saveTime(command.commandText);
            this->myClock->clock(true, false);
            this->isMinutePassed = true;
            this->display->resetHomeScreenFlags();
            if (this->hasSentDate)
            {
                this->mySerial.send("/mautomatic&" + command.chatId);
                this->firstTimePowerOn = false;
                this->hasSentDate = false;
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
                this->display->resetHomeScreenFlags();
                this->isMinutePassed = true;
                this->mySerial.send("/ddate&" + command.chatId);
                this->isLightAuto = true;
                this->isWaterAuto = true;
                this->isFanAuto = true;
                this->hasSentDate = true;
            }
            else if (command.commandText == "state")
            {
                this->sendState(command.chatId);
            }
            else if (command.commandText == "togglelight")
            {
                this->manualToggleLight();
                this->sendLightState(command.chatId);
            }
            else if (command.commandText == "togglewater")
            {
                this->manualToggleWater(command.chatId);
                this->sendWateringState(command.chatId);
            }
            else if (command.commandText == "togglefan")
            {
                this->manualToggleFan();
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

void Controller::connectionTimeOut()
{
    this->hasConnectionTimedOut = true;
    this->display->clear();
}

bool Controller::getConnectionState()
{
    return this->isConnectedToESP;
}

void Controller::start()
{
    Action action = this->navigator->getAction();

    if (!this->hasConnectionTimedOut)
    {
        if (!this->getConnectionState())
        {
            this->display->connecting();
        }
        else
        {
            if (this->firstTimePowerOn)
            {
                this->display->clear();
                this->mySerial.send("/ddate");
                this->hasSentDate = true;
            }
            else
            {
                this->automaticStart(action);
            }
        }
    }
    else
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
            this->automaticStart(action);
        }
    }

    serialEventRun1();
}

// private
void Controller::wait()
{
    long timer = millis();
    while (millis() - timer < 500)
        ;
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
    this->mySerial.send("/imode=" + String("automatic"));
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
    }
    else
    {
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
        this->mySerial.send("/iwaters=" + String((this->shouldWatering == false) ? "not " : ""));
        this->wait();
        this->sendEndMessage(chatId);
    }
    else
    {
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
    }
    else
    {
        this->mySerial.send("/ifan=" + String((this->shouldFan) ? "spinning" : "off"));
    }
}

void Controller::manualToggleLight()
{
    this->isLightAuto = false;
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
}

void Controller::manualToggleWater(const String &chatId = "")
{
    this->isWaterAuto = false;
    if (!this->shouldWatering)
    {
        this->navigator->waterOn();
        if (chatId != "")
        {
            this->mySerial.send("/iwateron&" + chatId);
        }
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
}

void Controller::manualToggleFan()
{
    this->isFanAuto = false;
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
}

void Controller::chooseTime(Action action)
{
    this->display->chooseTime(this->myClock->selectedDigit, this->myClock->digits);
    this->myClock->chooseTime(action);
}

void Controller::automaticStart(Action action)
{
    int soilSensorValue = this->soilSensor->readSensor();
    float soilSensorPercentage = this->soilSensor->valueToPercentage(soilSensorValue);

    if (action == Action_BDOWN)
    {
        this->waterCounter = ++this->waterCounter % 3;
#ifdef DEBUG
        Serial.println("Water counter: " + String(this->waterCounter));
#endif

        if (this->waterCounter == 0)
        {
            this->isWaterAuto = true;
            this->display->resetHomeScreenFlags();
#ifdef DEBUG
            Serial.println("Water automatic");
#endif
        }
        else
            this->manualToggleWater();
    }

    if (this->isWaterAuto)
    {
        this->shouldWatering = this->soilSensor->shouldWatering(soilSensorValue);
        if (this->shouldWatering)
        {
            this->navigator->waterOn();
        }
        else
        {
            this->navigator->waterOff();
        }
    }

    if (action == Action_SELECT)
    {
        this->fanCounter = ++this->fanCounter % 3;
#ifdef DEBUG
        Serial.println("Fan counter: " + String(this->fanCounter));
#endif

        if (this->fanCounter == 0)
        {
            this->isFanAuto = true;
            this->display->resetHomeScreenFlags();
#ifdef DEBUG
            Serial.println("Fan automatic");
#endif
        }
        else
            this->manualToggleFan();
    }

    float temperatureSensorValue = this->temperatureSensor->readDiet();
    if (this->isFanAuto)
    {
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

    if (action == Action_BUP)
    {
        this->lightCounter = ++this->lightCounter % 3;
#ifdef DEBUG
        Serial.println("Light counter: " + String(this->lightCounter));
#endif

        if (this->lightCounter == 0)
        {
            this->isLightAuto = true;
            this->display->resetHomeScreenFlags();
#ifdef DEBUG
            Serial.println("Light automatic");
#endif
        }
        else
            this->manualToggleLight();
    }

    if (this->isLightAuto)
    {
        this->shouldLight = (this->myClock->dayCycle == DayCycle_DAY);
        if (this->shouldLight)
        {
            this->navigator->lightOn();
        }
        else
        {
            this->navigator->lightOff();
        }
    }

    this->display->homeScreen(this->getConnectionState(), this->myClock->getTimeAsString(), this->isMinutePassed, (this->shouldLight ? DayCycle_DAY : DayCycle_NIGHT), soilSensorPercentage, temperatureSensorValue, this->shouldWatering, this->shouldFan, this->isLightAuto, this->isWaterAuto, this->isFanAuto);

    this->isMinutePassed = this->myClock->isMinutePassed(); // check if minute is really passed
    this->myClock->clock(this->isMinutePassed, true);

    if (!this->isWaterAuto && this->shouldWatering)
    {
        if (millis() - this->wateringTimer > 180000)
        { // 3 minutes = 180000
            this->shouldWatering = false;
            this->navigator->waterOff();
#ifdef DEBUG
            Serial.println("WATER OFF CAUSE OF TIMER");
#endif
        }
    }
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