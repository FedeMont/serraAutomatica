#ifndef __WIFICONFIGURATION_H__
#define __WIFICONFIGURATION_H__

#include <Arduino.h>
#include "Utils.h"
#include <ESP8266WiFi.h>

class WiFiConfiguration
{
private:
    int connection_timeout;
    long connection_timeout_timer;
    String ssid;
    String password;
public:
    WiFiConfiguration();
    WiFiConfiguration(String, String);
    ~WiFiConfiguration();

    void setCredentials(String, String);
    bool connect();
    bool connect(int [4], int[4], int[4], int [4]);

    void setAPMode();
    String getIpAddress();
};

#endif