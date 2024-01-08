#ifndef __WEBSERVER_H__
#define __WEBSERVER_H__

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include "FileSystem.h"
#include "Utils.h"

class WebServer {
private:
    ESP8266WebServer server;
    FileSystem *fileSystem;
    
    void save_json();
public:
    WebServer(FileSystem*);
    WebServer(FileSystem*, int);
    ~WebServer();

    bool begin();

    void handleClient();
};

#endif