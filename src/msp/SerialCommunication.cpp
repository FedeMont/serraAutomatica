#include "SerialCommunication.h"

SerialCommunication::SerialCommunication(/* args */)
{
}

SerialCommunication::~SerialCommunication()
{
}

void SerialCommunication::begin(unsigned long baud) {
    Serial1.begin(baud);

    this->send("/sSTART\n");
}

void SerialCommunication::send(const char *text) {
    Serial1.print(text);
}

String SerialCommunication::receive() {
    return Serial1.readStringUntil('\n');
}