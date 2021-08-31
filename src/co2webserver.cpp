#include "co2webserver.h"
#include "indexhtml.h"

CO2WebServer::CO2WebServer(): webServer_(80){
    webServer_.on("/", std::bind(&CO2WebServer::handleRoot, this));
    webServer_.on("/histories", std::bind(&CO2WebServer::handleGetHistories, this));
    webServer_.on("/calibrate", std::bind(&CO2WebServer::handleCalibrate, this));
}

void CO2WebServer::beginWebServer(){
    webServer_.begin();
    isWebServerBegun_ = true;
    Serial.println(F("Web Server started."));
}


void CO2WebServer::endWebServer(){
    webServer_.stop();
    isWebServerBegun_ = false;
    Serial.println(F("Web Server stopped."));
}

void CO2WebServer::beginCO2(){
    co2_.begin();
}


void CO2WebServer::tick(){
    co2_.tick();
    if(isWebServerBegun_){
        webServer_.handleClient();
    }
    else{
        if (WiFi.status() == WL_CONNECTED){
            beginWebServer();
            isWebServerBegun_ = true;
        }
    }
}

void CO2WebServer::handleRoot() {
    webServer_.send(200, "text/html", INDEX_HTML);
}

void CO2WebServer::handleGetHistories() {

    auto histories = co2_.co2Histories();

    if(histories->empty()){
        webServer_.send(200, "application/json", "{\"times\":[], \"co2Logs\":[], \"temperatureLogs\":[]}");
        return;
    }

    auto latestHistory = histories->back();
    auto latestTime = latestHistory.time();

    String response = "{\"times\":[";

    bool isFirst = true;
    for (std::list<CO2History>::const_iterator it = histories->begin(); it != histories->end(); ++it) {
        auto time = it->time();

        if(!isFirst){
            response += ",";
        }

        response += String(latestTime - time);
        isFirst = false;
    }

    response += "], \"co2Logs\":[";

    isFirst = true;
    for (std::list<CO2History>::const_iterator it = histories->begin(); it != histories->end(); ++it) {
        auto co2 = it->co2();

        if(!isFirst){
            response += ",";
        }
        
        response += String(co2);
        isFirst = false;
    }

    response += "], \"temperatureLogs\":[";

    isFirst = true;
    for (std::list<CO2History>::const_iterator it = histories->begin(); it != histories->end(); ++it) {
        auto temperature = it->temperature();

        if(!isFirst){
            response += ",";
        }
        
        response += String(temperature);
        isFirst = false;
    }

    response += "]}";


    webServer_.send(200, "application/json", response);
}


void CO2WebServer::handleCalibrate() {

    String contents = F(
    "<html><body>"
    "Calibratation command was sent to the CO2 Sensor."
    "</body></html>"
    );

    co2_.calibrate();

    webServer_.send(200, "text/html", contents);
}