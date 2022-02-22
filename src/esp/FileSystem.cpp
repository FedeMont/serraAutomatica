#include "FileSystem.h"

FileSystem::FileSystem()
{
}

FileSystem::~FileSystem()
{
}

bool FileSystem::begin()
{
    if (LittleFS.begin())
    {
#ifdef DEBUG
        Serial.println("File system montato.");
#endif
        return true;
    }
    else
    {
#ifdef DEBUG
        Serial.println("File system NON montato.");
#endif
        return false;
    }
}

bool FileSystem::read(const String &file_name, DynamicJsonDocument *json)
{
    File config_json_file = LittleFS.open(file_name, "r");
    String config_json_string = config_json_file.readStringUntil('\n');

//    const size_t capacity = JSON_OBJECT_SIZE(24) + 420;
//    DynamicJsonDocument jsonRead(capacity);
//    DynamicJsonDocument jsonError(capacity);

    DeserializationError errorRead = deserializeJson(*json, config_json_string);
    if (errorRead) {
#ifdef DEBUG
        Serial.print("deserializeJson() line255 failed: ");
        Serial.println(errorRead.c_str());
//        serializeJson(jsonError, Serial);
        Serial.println("");
        Serial.println("Impossibile leggere la configurazione.");
#endif
//        serializeJson(jsonError, *json);
        return false;
    } else {
#ifdef DEBUG
        Serial.println("Configurazione letta correttamente.");
        serializeJson(*json, Serial);
        Serial.println("");
#endif
//        serializeJson(jsonRead, *json);
        return true;
    }
}

void FileSystem::write(const String &file_name, DynamicJsonDocument &json)
{
    File config_json_file = LittleFS.open(file_name, "w");
    delay(200);
    serializeJson(json, config_json_file);
#ifdef DEBUG
    serializeJson(json, Serial);
    Serial.println("");
#endif
}