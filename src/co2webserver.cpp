#include "co2webserver.h"
#include "indexhtml.h"

CO2WebServer::CO2WebServer(): webServer_(80){
    webServer_.on("/", std::bind(&CO2WebServer::handleRoot, this));
    webServer_.on("/histories", std::bind(&CO2WebServer::handleGetHistories, this));
    webServer_.on("/calibrate", std::bind(&CO2WebServer::handleCalibrate, this));
    webServer_.on("/temperature", std::bind(&CO2WebServer::handleCalibrateTemperature, this));
    webServer_.on("/clear", std::bind(&CO2WebServer::handleClearHistories, this));
    webServer_.on("/ppm", std::bind(&CO2WebServer::handleGetC02Value, this));
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

    auto farHistories = co2_.farCo2Histories();
    auto nearHistories = co2_.nearCo2Histories();

    if(nearHistories->empty()){
        webServer_.send(200, "application/json", "{\"times\":[], \"co2Logs\":[], \"temperatureLogs\":[]}");
        return;
    }

    auto latestHistory = nearHistories->back();
    auto latestTime = latestHistory.time();

    String response = "{\"times\":[";

    bool isFirst = true;
    for (std::list<CO2History>::const_iterator it = farHistories->begin(); it != farHistories->end(); ++it) {
        auto time = it->time();

        if(!isFirst){
            response += ",";
        }

        response += String(latestTime - time);
        isFirst = false;
    }

    for (std::list<CO2History>::const_iterator it = nearHistories->begin(); it != nearHistories->end(); ++it) {
        auto time = it->time();

        if(!isFirst){
            response += ",";
        }

        response += String(latestTime - time);
        isFirst = false;
    }

    response += "], \"co2Logs\":[";

    isFirst = true;
    for (std::list<CO2History>::const_iterator it = farHistories->begin(); it != farHistories->end(); ++it) {
        auto co2 = it->co2();

        if(!isFirst){
            response += ",";
        }
        
        response += String(co2);
        isFirst = false;
    }


    for (std::list<CO2History>::const_iterator it = nearHistories->begin(); it != nearHistories->end(); ++it) {
        auto co2 = it->co2();

        if(!isFirst){
            response += ",";
        }
        
        response += String(co2);
        isFirst = false;
    }

    response += "], \"temperatureLogs\":[";

    isFirst = true;
    for (std::list<CO2History>::const_iterator it = farHistories->begin(); it != farHistories->end(); ++it) {
        auto temperature = it->temperature();

        if(!isFirst){
            response += ",";
        }
        
        response += String(temperature);
        isFirst = false;
    }

    for (std::list<CO2History>::const_iterator it = nearHistories->begin(); it != nearHistories->end(); ++it) {
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



void CO2WebServer::handleCalibrateTemperature(){

    if(webServer_.hasArg("value")){

        String contents = "<html><body>Temperature calibrated. value: ";
        
        float v = webServer_.arg("value").toFloat();
        co2_.setTemperatureCalibrationValue(v);

        contents += String(v);
        contents += "</body></html>";

        webServer_.send(200, "text/html", contents);
    }
    else{

        String contents = F(
        "<html><body>"
        "Please set \"value\" param for temperature calibration."
        "</body></html>"
        );

        webServer_.send(200, "text/html", contents);

    }

}


void CO2WebServer::handleClearHistories() {

    String contents = F(
    "<html><body>"
    "Histories are cleared."
    "</body></html>"
    );

    co2_.clearHistories();

    webServer_.send(200, "text/html", contents);
}



void CO2WebServer::handleGetC02Value(){
    webServer_.send(200, "text/plain", String(co2_.currentValue()));
}
