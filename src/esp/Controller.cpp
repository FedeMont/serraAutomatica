#include "Controller.h"

Controller::Controller() : webServer(&this->fileSystem), botHandler(&this->fileSystem)
{
}

Controller::~Controller()
{
}

void Controller::begin()
{
    pinMode(D4, OUTPUT);
    Serial.begin(115200);
#ifdef DEBUG
    Serial.println("\n");
#endif

    this->fileSystem = FileSystem();
    this->wiFiConfiguration = WiFiConfiguration();
    // this->webServer = WebServer(&this->fileSystem);
    // this->botHandler = BotHandler(&this->fileSystem);
    this->navigator = Navigator();
    this->soilSensor = SoilSensor();
    this->myClock = MyClock();

    this->lightState = State_AUTO;
    this->fanState = State_AUTO;
    this->waterState = State_AUTO;

    this->fileSystem.begin();

    const size_t capacity = JSON_OBJECT_SIZE(24) + 420;
    DynamicJsonDocument json(capacity);

    if (this->fileSystem.read("/config.json", &json))
    {
        this->deserializeJson(json);
        this->wiFiConfiguration.setCredentials(this->net_ssid, this->net_psw);
    }

    bool isWifiConnected =
        (this->is_config_static)?
        this->wiFiConfiguration.connect(this->ip, this->dns, this->default_gw, this->subnet_mask)
        :
        this->wiFiConfiguration.connect()
        ;

    if (!isWifiConnected)
    {
        this->wiFiConfiguration.setAPMode();
    }

    this->webServer.begin();

    this->botHandler.begin(this->telegram_chat_id, &this->lightState, &this->fanState, &this->waterState, &this->manualWateringTimer, this->wiFiConfiguration.getIpAddress());
    this->botHandler.setCommands();

    this->navigator.begin();
    this->myClock.begin();

    this->soilSensorPin = this->navigator.getSoilSensorPin();
}

void Controller::start()
{
    this->webServer.handleClient();
    this->myClock.clock();

    DayCycle dayCycle = this->myClock.getDayCycle();
    int soilSensorValue = this->soilSensor.readSensor(this->soilSensorPin);
    float soilSensorPercentage = this->soilSensor.valueToPercentage(soilSensorValue);
    bool shouldWatering = this->soilSensor.shouldWatering(soilSensorValue);

    this->botHandler.start(dayCycle, soilSensorPercentage, shouldWatering);

    this->autoStart(dayCycle, shouldWatering);
}

// private
void  Controller::deserializeJson(DynamicJsonDocument &json)
{
    this->net_ssid = json["net_ssid"];
    this->net_psw = json["net_pswd"];

    this->is_config_static = String(json["net_static"]) == "true";

    this->ip[0] = json["net_ip_0"];
    this->ip[1] = json["net_ip_1"];
    this->ip[2] = json["net_ip_2"];
    this->ip[3] = json["net_ip_3"];

    this->default_gw[0] = json["net_dfgw_0"];
    this->default_gw[1] = json["net_dfgw_1"];
    this->default_gw[2] = json["net_dfgw_2"];
    this->default_gw[3] = json["net_dfgw_3"];

    this->subnet_mask[0] = json["net_sm_0"];
    this->subnet_mask[1] = json["net_sm_1"];
    this->subnet_mask[2] = json["net_sm_2"];
    this->subnet_mask[3] = json["net_sm_3"];

    this->dns[0] = json["net_dns_0"];
    this->dns[1] = json["net_dns_1"];
    this->dns[2] = json["net_dns_2"];
    this->dns[3] = json["net_dns_3"];

    this->telegram_chat_id = String(json["telegram_chat_id"]);
}

void Controller::autoStart(DayCycle dayCycle, bool shouldWatering)
{
    switch (this->lightState)
    {
    case State_AUTO:
        if (dayCycle == DayCycle_DAY)
            this->navigator.lightOn();
        else
            this->navigator.lightOff();
        break;
    case State_ON:
        this->navigator.lightOn();
        break;
    case State_OFF:
        this->navigator.lightOff();
        break;
    default:
        break;
    }

    switch (this->fanState)
    {
    case State_AUTO:
        if (dayCycle == DayCycle_DAY)
            this->navigator.fanOn();
        else
            this->navigator.fanOff();
        break;
    case State_ON:
        this->navigator.fanOn();
        break;
    case State_OFF:
        this->navigator.fanOff();
        break;
    default:
        break;
    }

    switch (this->waterState)
    {
    case State_AUTO:
        if (shouldWatering)
            this->navigator.waterOn();
        else
            this->navigator.waterOff();
        break;
    case State_ON:
        if (millis() - this->manualWateringTimer > 180000) // 3 minuti = 3 * 60s * 1000ms = 180000ms
        {
            this->waterState = State_OFF;
        }
        else
        {
            this->navigator.waterOn();
        }
        
        break;
    case State_OFF:
        this->navigator.waterOff();
        break;
    default:
        break;
    }
}