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
public:
    SerialCommunication(/* args */);
    ~SerialCommunication();

    void flush();
#ifndef Energia_h
    int available();
    void begin(uint32_t); // ESP8266
    String readStringUntil(const char &);
#else
    void begin(unsigned long); // MSP
#endif

    void send(const String &);
    Command commandParser(const String &);
private:
#ifndef Energia_h
    SoftwareSerial mySerial; // ESP8266
#endif
};

#endif