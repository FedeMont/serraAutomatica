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
    Serial1.print(String(text + '@'));
}

Command SerialCommunication::receive() {
    String message = Serial1.readStringUntil('@');
    Serial1.flush();
    // message = message.substring(0, message.length());

#ifdef DEBUG
    Serial.print("Complete message: ");
    Serial.println(message);
#endif

    String type = message.substring(0, 2).c_str();
    String text = "";
    String chatId = "";
    if (message.indexOf("::") == -1) { // nothing found
        text = message.substring(2);
        chatId = "";
    } else {
        text = message.substring(2, message.indexOf("::"));
        chatId = message.substring(message.indexOf("::") + 2);
    }

#ifdef DEBUG
    Serial.println("RECEIVED");
    Serial.println(type);
    Serial.println(text);
    Serial.println(chatId);
#endif

    Command command;
    if (type.charAt(0) == '/') {
        command.isValid = true;
        command.commandType = type.charAt(1);
        command.commandText = text;
        command.chatId = chatId;
    }
    else {
        command.isValid = false;
    }

    return command;
}