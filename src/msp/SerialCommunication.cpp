#include "SerialCommunication.h"

SerialCommunication::SerialCommunication(/* args */)
{
}

SerialCommunication::~SerialCommunication()
{
}

void SerialCommunication::begin(unsigned long baud) {
    Serial1.begin(baud);

    this->send("/sSTART");
}

void SerialCommunication::send(const String &text) {
    Serial1.println(text);
}

Command SerialCommunication::receive() {
    String message = Serial1.readStringUntil('\n');
    const char* type = message.substring(0, 2).c_str();

    Command command;
    if (type[0] == '/') {
        command.isValid = true;
        command.commandType = type[1];
        command.commandText = message.substring(2);
    }
    else {
        command.isValid = false;
    }

    return command;
}