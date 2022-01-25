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

    this->send("/sSTART");
}

void SerialCommunication::send(const String &text) {
    String message = String(text + '@');
#ifdef DEBUG
    Serial.print("Sending message: ");
    Serial.println(message);
#endif
    Serial1.print(message);
}

Command SerialCommunication::receive() {
    // char msg[255];
    // strcpy(msg, Serial1.readStringUntil('@').c_str());

    String message = String(Serial1.readStringUntil('@'));
    message.trim();
    this->flush();

    // message = message.substring(0, message.length());

#ifdef DEBUG
    Serial.print("Complete message: ");
    Serial.println(message);
#endif

    String type = String(message.substring(0, 2));
    // (message.substring(2)).toCharArray(msg, (message.substring(2)).length() + 1);
    String text = "";
    String chatId = "";

    if (type != "/i") {
        text = message.substring(2);
    } else {
        text = message.substring(2, 7);
        chatId = message.substring(8);
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

    // this->flush();

    return command;
}