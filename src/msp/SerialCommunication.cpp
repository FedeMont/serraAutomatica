#include <Energia.h>
#include "SerialCommunication.h"

SerialCommunication::SerialCommunication(/* args */)
{
}

SerialCommunication::~SerialCommunication()
{
}

// public
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
    String msg = completeMessage.substring(2);
    String text;
    String chatId = "";

    if (msg.indexOf('&') > 0)
    {
        text = msg.substring(0, msg.indexOf('&'));
        chatId = msg.substring(msg.indexOf('&') + 1);
    }
    else 
    {
        text = msg;
    }

#ifdef DEBUG
    Serial.println("RECEIVED");
    Serial.println(completeMessage);
    Serial.println(type);
    Serial.println(text);
    Serial.println(chatId);
#endif

    if (type.charAt(0) == '/')
    {
        command.isValid = true;
        command.commandType = type.charAt(1);
        command.commandText = text;
        command.chatId = chatId;
    }
    else
    {
        command.isValid = false;
    }

    return command;
}