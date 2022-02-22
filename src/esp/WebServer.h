#ifndef __WEBSERVER_H__
#define __WEBSERVER_H__

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include "FileSystem.h"
#include "Types.h"

class WebServer {
private:
    ESP8266WebServer server;
    FileSystem *fileSystem;

    void save_json();
public:
    WebServer(FileSystem *fileSystem);
    WebServer(FileSystem *fileSystem, int);
    ~WebServer();

    void begin();

    void handleClient();
};

#endif