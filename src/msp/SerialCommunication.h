#ifndef __SERIALCOMMUNICATION_H__
#define __SERIALCOMMUNICATION_H__

#include "Energia.h"
#include "types.h"

class SerialCommunication
{
private:
public:
    SerialCommunication(/* args */);
    ~SerialCommunication();

    void flush();
    void begin(unsigned long);

    void send(const String &);
    Command receive();

    String readFrom();
    void writeTo();
};

#endif
