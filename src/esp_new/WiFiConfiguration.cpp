#include "WiFiConfiguration.h"

WiFiConfiguration::WiFiConfiguration()
{
    this->connection_timeout = 15000;
}

WiFiConfiguration::WiFiConfiguration(String ssid, String psw)
{
    this->ssid = ssid;
    this->password = psw;
    this->connection_timeout = 15000;
}

WiFiConfiguration::~WiFiConfiguration()
{
}

void WiFiConfiguration::setCredentials(String ssid, String password)
{
    this->ssid = ssid;
    this->password = password;
}

bool WiFiConfiguration::connect()
{
    // Connect to Wi-Fi
    WiFi.mode(WIFI_STA);
    WiFi.begin(this->ssid, this->password);

    this->connection_timeout_timer = millis();
    while (WiFi.status() != WL_CONNECTED)
    {
#ifdef DEBUG
        Serial.println(F("Connecting to WiFi..."));
#endif

        digitalWrite(D4, HIGH);
        delay(500);
        digitalWrite(D4, LOW);
        delay(500); 

        if (millis() - this->connection_timeout_timer > this->connection_timeout)
        {
#ifdef DEBUG
            Serial.println(F("Connection timed out."));
#endif
            return false;
        }
    }

    #ifdef DEBUG
    Serial.println(this->getIpAddress());
#endif
    digitalWrite(D4, LOW);
    delay(2000);
    digitalWrite(D4, HIGH);

    return true;
}

bool WiFiConfiguration::connect(int ip[4], int dns[4], int gw[4], int sm[4])
{
#ifdef DEBUG
    Serial.println(F("Configurazione statica..."));
#endif

    IPAddress ip_addr(ip[0], ip[1], ip[2], ip[3]);
    IPAddress dns_addr(dns[0], dns[1], dns[2], dns[3]);
    IPAddress gw_addr(gw[0], gw[1], gw[2], gw[3]);
    IPAddress sm_addr(sm[0], sm[1], sm[2], sm[3]);

#ifdef DEBUG
    Serial.print(F("IP: "));
    Serial.println(ip_addr);
    Serial.print(F("Gateway: "));
    Serial.println(gw_addr);
    Serial.print(F("Subnet: "));
    Serial.println(sm_addr);
    Serial.print(F("DNS: "));
    Serial.println(dns_addr);
#endif

    if (!WiFi.config(ip_addr, gw_addr, sm_addr, dns_addr)) { // Configurazione statica del web-server in caso di Client
        Serial.println(F("Errore nella configurazione statica."));
        return false;
    }

    return this->connect();
}

void WiFiConfiguration::setAPMode()
{
    WiFi.disconnect(true);
    WiFi.mode(WIFI_AP);

    WiFi.softAP("Automatic-Green-House", "AutomaticGreenHouse");

#ifdef DEBUG
    Serial.print(F("Access Point Mode: "));
    Serial.println(this->getIpAddress());
#endif
}

String WiFiConfiguration::getIpAddress()
{
    switch (WiFi.getMode())
    {
    case WIFI_STA:
        return WiFi.localIP().toString();
        break;
    case WIFI_AP:
        return WiFi.softAPIP().toString();
        break;
    default:
        return "";
        break;
    }
}