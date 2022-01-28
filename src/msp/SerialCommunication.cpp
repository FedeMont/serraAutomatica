#include <Energia.h>
#include "SerialCommunication.h"

SerialCommunication::SerialCommunication(/* args */)
{
}

SerialCommunication::~SerialCommunication()
{
}

void SerialCommunication::flush() {
    while (Serial1.available() > 0) {
        char t = Serial1.read();
    }
}

void SerialCommunication::begin(unsigned long baud) {
    Serial1.begin(baud);
}

void SerialCommunication::send(const String &text) {
    String message = String(text + '@');
#ifdef DEBUG
    Serial.print("Sending message: ");
    Serial.println(message);
#endif
    Serial1.print(message);
}

Command SerialCommunication::commandParser(const String &completeMessage) {
    Command command;

    String type = completeMessage.substring(0, 2);
    String text = completeMessage.substring(2);

#ifdef DEBUG
    Serial.println("RECEIVED");
    Serial.println(type);
    Serial.println(text);
#endif

    if (type.charAt(0) == '/') {
        command.isValid = true;
        command.commandType = type.charAt(1);
        command.commandText = text;
    }
    else {
        command.isValid = false;
    }

    return command;
}