#ifndef __WIFICONFIGURATION_H__
#define __WIFICONFIGURATION_H__

#include <Arduino.h>
#include <ESP8266WiFi.h>

#include "Types.h"

class WiFiConfiguration
{
private:
    const char *ssid;
    const char *password;

public:
    WiFiConfiguration(const char *, const char *);
    ~WiFiConfiguration();
    
    void connect();
};

#endif