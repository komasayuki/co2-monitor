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



    if(WIFI_DEFAULT_ON){
        wifiConnection.begin();
        wifiConnection.tick();
    }

    co2WebServer.beginCO2();
    delay(500);

    RotatedDisplay display;
    display.begin();

}


void loop()
{
    
    wifiConnection.tick();
    co2WebServer.tick();

    static bool usesWifi = WIFI_DEFAULT_ON;
    static bool wifiButtonEngaged = false;

    M5.update();
    // handling WiFi ON/OFF by long pressing the button
    if (M5.Btn.pressedFor(1000) && !wifiButtonEngaged){
        
        usesWifi = !usesWifi;
        wifiButtonEngaged = true;

        if(usesWifi){
            wifiConnection.begin();
            M5.dis.fillpix(0xff0000); //GRB green
            Serial.println(F("WiFi ON."));
        }
        else{
            wifiConnection.end();
            M5.dis.fillpix(0xffff00); //GRB yellow
            Serial.println(F("WiFi OFF."));
        }

    }

    if(M5.Btn.releasedFor(1000)){
        wifiButtonEngaged = false;
    }

    delay(50);
}






