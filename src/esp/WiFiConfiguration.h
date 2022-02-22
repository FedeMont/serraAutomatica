#ifndef __WIFICONFIGURATION_H__
#define __WIFICONFIGURATION_H__

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "Types.h"

class WiFiConfiguration
{
private:
    long connection_timeout_timer;
    const char *ssid;
    const char *password;
public:
    WiFiConfiguration();
    WiFiConfiguration(const char *, const char *);
    ~WiFiConfiguration();

    void setCredentials(const char *, const char *);
    bool connect();
    bool connect(int [4], int[4], int[4], int [4]);

    void setAPMode();
    String getIpAddress();
};

#endif