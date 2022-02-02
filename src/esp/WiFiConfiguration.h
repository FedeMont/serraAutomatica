#ifndef __WIFICONFIGURATION_H__
#define __WIFICONFIGURATION_H__

#include <Arduino.h>
#include <ESP8266WiFi.h>

#include "Types.h"

class WiFiConfiguration
{
public:
    WiFiConfiguration(const char *, const char *);
    ~WiFiConfiguration();

    void connect();
private:
    const char *ssid;
    const char *password;
};

#endif