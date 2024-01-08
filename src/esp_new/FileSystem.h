#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

#include <Arduino.h>
#include "Utils.h"
#include "LittleFS.h"
#include "ArduinoJson.h"

class FileSystem
{
private:
public:
    FileSystem();
    ~FileSystem();

    bool begin();
    bool read(const String&, DynamicJsonDocument *);
    void write(const String&, DynamicJsonDocument &);
};

#endif