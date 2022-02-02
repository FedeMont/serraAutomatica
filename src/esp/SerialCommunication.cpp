#include "SerialCommunication.h"

SerialCommunication::SerialCommunication(/* args */) : mySerial(13, 15) // d7 rx bianco, d8 tx marrone
{
}

SerialCommunication::~SerialCommunication()
{
}

// public
int SerialCommunication::available()
{
    return this->mySerial.available();
}

void SerialCommunication::flush()
{
    while (this->available() > 0)
    {
        char t = this->mySerial.read();
    }
}

void SerialCommunication::begin(uint32_t baud)
{
    this->mySerial.begin(baud);
}

String SerialCommunication::readStringUntil(const char &terminator)
{
    return this->mySerial.readStringUntil(terminator);
}

void SerialCommunication::send(const String &text)
{
    String message = String(text + '@');
#ifdef DEBUG
    Serial.print("Sending message: ");
    Serial.println(message);
#endif
    this->mySerial.print(message);
}

Command SerialCommunication::commandParser(const String &completeMessage)
{
    Command command;
    String type = completeMessage.substring(0, 2);
    String msg = completeMessage.substring(2);
    String textAndValue = msg.substring(0, msg.indexOf('&'));
    String text = "";
    String value = "";

    String chatId = "";
    if (msg.indexOf('&') > 0)
    {
        chatId = msg.substring(msg.indexOf('&') + 1);
    }

    if (type == "/i" && textAndValue.indexOf('=') > 0)
    {
        text = textAndValue.substring(0, msg.indexOf('='));
        value = textAndValue.substring(msg.indexOf('=') + 1);
    }
    else
    {
        text = textAndValue;
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
        command.commandValue = value;
        command.chatId = chatId;
    }
    else
    {
        command.isValid = false;
    }

    return command;
}
