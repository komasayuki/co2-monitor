#include "co2webserver.h"
#include "wificonnection.h"
#include "config.h"

#include <M5Atom.h>

static CO2WebServer co2WebServer;
static WifiConnection wifiConnection;

#include "rotateddisplay.h"

void setup()
{
    M5.begin(true, false, true);
    delay(50);

    RotatedDisplay display;
    display.begin();

    if(WIFI_DEFAULT_ON){
        wifiConnection.begin();
        wifiConnection.tick();
    }

    co2WebServer.beginCO2();

}


void loop()
{
    
    wifiConnection.tick();
    co2WebServer.tick();

    static bool usesWifi = WIFI_DEFAULT_ON;

    M5.update();
    if (M5.Btn.wasPressed()){
        
        usesWifi = !usesWifi;

        if(usesWifi){
            wifiConnection.begin();
            Serial.println(F("WiFi ON."));
        }
        else{
            wifiConnection.end();
            Serial.println(F("WiFi OFF."));
        }

    }

    delay(50);
}






