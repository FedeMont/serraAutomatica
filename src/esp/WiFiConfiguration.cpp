#include "WiFiConfiguration.h"

WiFiConfiguration::WiFiConfiguration(const char *ssid, const char *psw)
{
    this->ssid = ssid;
    this->password = psw;
}

WiFiConfiguration::~WiFiConfiguration()
{
}

// public
void WiFiConfiguration::connect()
{
    // Connect to Wi-Fi
    WiFi.mode(WIFI_STA);
    WiFi.begin(this->ssid, this->password);

    while (WiFi.status() != WL_CONNECTED)
    {
#ifdef DEBUG
        Serial.println("Connecting to WiFi..");
#endif
        digitalWrite(2, HIGH);
        delay(500);
        digitalWrite(2, LOW);
        delay(500);
    }

// Print ESP32 Local IP Address
#ifdef DEBUG
    Serial.println(WiFi.localIP());
#endif
    digitalWrite(2, LOW);
    delay(2000);
    digitalWrite(2, HIGH);
}