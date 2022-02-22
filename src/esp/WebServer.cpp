#include "WebServer.h"

WebServer::WebServer(FileSystem *fileSystem) : server(80)
{
    this->fileSystem = fileSystem;
}

WebServer::WebServer(FileSystem *fileSystem, int port) : server(port)
{
    this->fileSystem = fileSystem;
}

WebServer::~WebServer()
{}

void WebServer::begin()
{
    this->server.on("/save", [&](){this->save_json();});
    this->server.serveStatic("/img", LittleFS, "/img");
    this->server.serveStatic("/css", LittleFS, "/css");
    this->server.serveStatic("/", LittleFS, "/index.html");
    this->server.begin();
}

void WebServer::save_json()
{
    Serial.println("SAVE");
    if (!LittleFS.begin()) { // controllo di aver accesso al filesystem
        // Se viene visualizzato c'è un problema al filesystem
        this->server.send(500, "text/plain", "File system mount failed"); // messaggio di callback per client web
        Serial.println("LittleFs Mount failed");
        return;
    } else { // File system correttamente caricato
        Serial.println("LittleFs Mount succesfull");
    }

    const size_t capacity = JSON_OBJECT_SIZE(24) + 420;
    DynamicJsonDocument json(capacity);
    if(!this->fileSystem->read("/config.json", &json))
    {
#ifdef DEBUG
        Serial.println("Errore nella lettura della configurazione.");
#endif
    }

    // #######################################################
    // controllo e salvataggio dei dati in caso di cambiamento
    // #######################################################

    if (this->server.arg("net_ssid") != "") { // ssid
        json["net_ssid"] = this->server.arg("net_ssid");
    }

    if (this->server.arg("net_pswd") != "") { // password
        json["net_pswd"] = this->server.arg("net_pswd");
    }

    if(this->server.arg("watering_timer") != "") { // watering timer
        json["watering_timer"] = this->server.arg("watering_timer");
    }

    if (this->server.arg("net_static") != "") { // 1-0 abilita configurazione statica
        json["net_static"] = this->server.arg("net_static");
    }

    if (this->server.arg("net_ip_0") != "") { // ip[0]
        json["net_ip_0"] = this->server.arg("net_ip_0");
    }

    if (this->server.arg("net_ip_1") != "") { // ip[1]
        json["net_ip_1"] = this->server.arg("net_ip_1");
    }

    if (this->server.arg("net_ip_2") != "") { // ip[2]
        json["net_ip_2"] = this->server.arg("net_ip_2");
    }

    if (this->server.arg("net_ip_3") != "") { // ip[3]
        json["net_ip_3"] = this->server.arg("net_ip_3");
    }

    if (this->server.arg("net_dns_0") != "") { // dns[0]
        json["net_dns_0"] = this->server.arg("net_dns_0");
    }

    if (this->server.arg("net_dns_1") != "") { // dns[1]
        json["net_dns_1"] = this->server.arg("net_dns_1");
    }

    if (this->server.arg("net_dns_2") != "") { // dns[2]
        json["net_dns_2"] = this->server.arg("net_dns_2");
    }

    if (this->server.arg("net_dns_3") != "") { // dns[3]
        json["net_dns_3"] = this->server.arg("net_dns_3");
    }

    if (this->server.arg("net_dfgw_0") != "") { // dfgw[0]
        json["net_dfgw_0"] = this->server.arg("net_dfgw_0");
    }

    if (this->server.arg("net_dfgw_1") != "") { // dfgw[1]
        json["net_dfgw_1"] = this->server.arg("net_dfgw_1");
    }

    if (this->server.arg("net_dfgw_2") != "") { // dfgw[2]
        json["net_dfgw_2"] = this->server.arg("net_dfgw_2");
    }

    if (this->server.arg("net_dfgw_3") != "") { // dfgw[3]
        json["net_dfgw_3"] = this->server.arg("net_dfgw_3");
    }

    if (this->server.arg("net_sm_0") != "") { // sm[0]
        json["net_sm_0"] = this->server.arg("net_sm_0");
    }

    if (this->server.arg("net_sm_1") != "") { // sm[1]
        json["net_sm_1"] = this->server.arg("net_sm_1");
    }

    if (this->server.arg("net_sm_2") != "") { // sm[2]
        json["net_sm_2"] = this->server.arg("net_sm_2");
    }

    if (this->server.arg("net_sm_3") != "") { // sm[3]
        json["net_sm_3"] = this->server.arg("net_sm_3");
    }

    delay(100); // aspetto che tutto sia correttamente settato e poi scrivo

    this->fileSystem->write("/config.json", json);

    this->server.send(200, "text/plain", "Salvataggio effettuato correttamente. Riavvia il dispositivo."); // messaggio di callback per client web
    Serial.println("");
    Serial.println("Riavvia il dispositivo.");
}

void WebServer::handleClient()
{
    this->server.handleClient();
}