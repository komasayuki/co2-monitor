#pragma once

#include "co2.h"
#include <WebServer.h>

class CO2WebServer{

public:
    explicit CO2WebServer();
    void beginWebServer();
    void endWebServer();
    void beginCO2();
    void tick();

private:
    void handleRoot();
    void handleGetHistories();
    void handleCalibrate();
    void handleCalibrateTemperature();
    void handleClearHistories();
    void handleGetC02Value();

    WebServer webServer_;
    CO2 co2_;
    bool isWebServerBegun_ = false;

};

