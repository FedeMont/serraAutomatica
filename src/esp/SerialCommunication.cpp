#include "SerialCommunication.h"

SerialCommunication::SerialCommunication(/* args */) : mySerial(13, 15) // d7 rx bianco, d8 tx marrone
{
}

SerialCommunication::~SerialCommunication()
{
}

void SerialCommunication::begin(uint32_t baud) {
    this->mySerial.begin(baud);

    this->send("/sSTART");
}

bool SerialCommunication::isAvailable() {
    return this->mySerial.available() > 0;
}

void SerialCommunication::send(const String &text) {
    this->mySerial.println(text);
}

Command SerialCommunication::receive() {
    String message = this->mySerial.readStringUntil('\n');
    // const char* type = message.substring(0, 2).c_str();

    Command command;
    if (message.substring(0, 2).c_str()[0] == '/') {
        command.isValid = true;
        command.commandType = message.substring(0, 2).c_str()[1];
        command.commandText = message.substring(2);
    }
    else {
        command.isValid = false;
    }

    return command;
}
