#include "SerialCommunication.h"

SerialCommunication::SerialCommunication(/* args */) : mySerial(13, 15) // d7 rx bianco, d8 tx marrone
{
}

SerialCommunication::~SerialCommunication()
{
}

void SerialCommunication::begin(uint32_t baud) {
    this->mySerial.begin(baud);

    this->send("/sSTART\n");
}

void SerialCommunication::send(const char *text) {
    this->mySerial.write(text);
}

String SerialCommunication::receive() {
    return this->mySerial.readStringUntil('\n');
}