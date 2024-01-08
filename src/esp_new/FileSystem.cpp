#include "FileSystem.h"

FileSystem::FileSystem()
{
}

FileSystem::~FileSystem()
{
}

bool FileSystem::begin()
{
    return LittleFS.begin();
}

bool FileSystem::read(const String &file_path, DynamicJsonDocument *json)
{
    File file = LittleFS.open(file_path, "r");
    String file_content = file.readStringUntil('\n');

    DeserializationError error = deserializeJson(*json, file_content);
    if (error) {
#ifdef DEBUG
        Serial.print(F("deserializeJson() line 24 failed: "));
        Serial.println(error.c_str());
        Serial.println(F("Impossibile leggere la configurazione."));
#endif
        return false;
    } else {
#ifdef DEBUG
        Serial.println(F("Configurazione letta correttamente."));
        serializeJson(*json, Serial);
        Serial.println("");
#endif
        return true;
    }
}

void FileSystem::write(const String &file_path, DynamicJsonDocument &json)
{
    File file = LittleFS.open(file_path, "w");
    delay(200);
    serializeJson(json, file);
#ifdef DEBUG
    serializeJson(json, Serial);
    Serial.println("");
#endif
    file.close();
}