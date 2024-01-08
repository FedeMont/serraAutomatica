#include "Controller.h"

Controller::Controller(/* args */) : webServer(&this->fs)
{
}

Controller::~Controller()
{
}

bool Controller::begin()
{
    pinMode(D4, OUTPUT);

    this->fs = FileSystem();

    if(!this->fs.begin()) 
    {
        Serial.println(F("Error initializing file system."));
        return false;
    } 
    else 
    {
        Serial.println(F("File system initialized."));
    }

    const size_t capacity = JSON_OBJECT_SIZE(24) + 420;
    DynamicJsonDocument json(capacity);
    if (this->fs.read("/config.json", &json))
    {
        this->deserializeJson(json);
        this->wifi.setCredentials(this->net_ssid, this->net_psw);
    }

    this->wifi = WiFiConfiguration();

    bool isWifiConnected =
        (this->isConfigStatic)?
        this->wifi.connect(this->ip, this->dns, this->default_gw, this->subnet_mask)
        :
        this->wifi.connect()
        ;

    if (!isWifiConnected)
    {
        this->wifi.setAPMode();
    }

    // this->webServer = WebServer(&this->fs);
    if (!this->webServer.begin())
    {
        Serial.println(F("Error initializing web server."));
        return false;
    }
    else
    {
        Serial.println(F("Web server initialized."));
    }

    return true;
}

void Controller::start()
{
    this->webServer.handleClient();
    yield();

}

void  Controller::deserializeJson(DynamicJsonDocument &json)
{
    this->net_ssid = String(json["net_ssid"]);
    this->net_psw = String(json["net_pswd"]);
    this->wateringTimerDelay = json["watering_timer"];

    this->isConfigStatic = String(json["net_static"]) == "true";

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