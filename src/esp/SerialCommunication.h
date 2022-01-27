#ifndef __SERIALCOMMUNICATION_H__
#define __SERIALCOMMUNICATION_H__

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "Types.h"

// class SoftwareSerial;

class SerialCommunication
{
private:
    SoftwareSerial mySerial;
public:
    SerialCommunication(/* args */);
    ~SerialCommunication();

    void flush();
    void begin(uint32_t);

    int available();
    String readStringUntil(const char &);
    
    void send(const String &);
    Command commandParser(const String &);
};

#endif
