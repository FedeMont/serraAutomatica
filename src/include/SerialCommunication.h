#ifndef __SERIALCOMMUNICATION_H__
#define __SERIALCOMMUNICATION_H__

#ifdef Energia_h
#include <Energia.h>
#else
#include <Arduino.h>
#include <SoftwareSerial.h>
#endif

#include "Types.h"

class SerialCommunication
{
private:
#ifndef Energia_h
    SoftwareSerial mySerial; // ESP8266
#endif
public:
    SerialCommunication(/* args */);
    ~SerialCommunication();

    void flush();
#ifndef Energia_h
    void begin(uint32_t); // ESP8266
    int available();
    String readStringUntil(const char &);
#else
    void begin(unsigned long); // MSP
#endif

    void send(const String &);
    Command commandParser(const String &);
};

#endif