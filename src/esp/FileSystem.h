#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

#include <Arduino.h>
#include "LittleFS.h"
#include "ArduinoJson.h"
#include "Types.h"

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

#endif //__FILESYSTEM_H__
