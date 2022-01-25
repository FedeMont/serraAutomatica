#include "SerialCommunication.h"

SerialCommunication::SerialCommunication(/* args */) : mySerial(13, 15) // d7 rx bianco, d8 tx marrone
{
}

SerialCommunication::~SerialCommunication()
{
}

void SerialCommunication::flush() {
    while (this->mySerial.available() > 0) {
        char t = this->mySerial.read();
    }
}

void SerialCommunication::begin(uint32_t baud) {
    this->mySerial.begin(baud);

    this->send("/sSTART");
}

bool SerialCommunication::isAvailable() {
    return this->mySerial.available() > 0;
}

void SerialCommunication::send(const String &text) {
    String message = String(text + '@');
#ifdef DEBUG
    Serial.print("Sending message: ");
    Serial.println(message);
#endif
    this->mySerial.print(message);
}

Command SerialCommunication::receive() {
    String message = this->mySerial.readStringUntil('@');
    // this->mySerial.flush();
    // message = message.substring(0, message.length());

#ifdef DEBUG
    Serial.print("Complete message: ");
    Serial.println(message);
#endif

    String type = message.substring(0, 2);
    String text = message.substring(2);
    // String chatId = "";
    // if (message.indexOf("\-") == -1) { // nothing found
        // text = message.substring(2);
        // chatId = "";
    // } else {
    //     text = message.substring(2, message.indexOf("\-"));
    //     chatId = message.substring(message.indexOf("\-") + 2);
    // }

#ifdef DEBUG
    Serial.println("RECEIVED");
    Serial.println(type);
    Serial.println(text);
    // Serial.println(chatId);
#endif

    Command command;
    if (type.charAt(0) == '/') {
        command.isValid = true;
        command.commandType = type.charAt(1);
        command.commandText = text;
        // command.chatId = chatId;
    }
    else {
        command.isValid = false;
    }

    this->flush();

    return command;
}
