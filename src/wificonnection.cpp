#include "wificonnection.h"
#include "config.h"

#include <M5Atom.h>
#include <WiFi.h>
#include <ESPmDNS.h>

void WifiConnection::tick(){

    if(!isBegun_){
        return;
    }

    auto status = WiFi.status();

    if (status == WL_CONNECTED){

        if(isConnected_){
            return;
        }

        isConnecting_ =  false;
        isConnected_ = true;

        Serial.println(F("connected."));
        Serial.print(F("IP: "));
        Serial.println(WiFi.localIP());

        if (MDNS.begin(MDNS_NAME)) {
            Serial.print(F("mDNS started: "));
            Serial.print(MDNS_NAME);
            Serial.println(F(".local"));
        }
        else{
            Serial.println("Error setting up MDNS responder!");
        }
        
        return;
    }

    if(isConnecting_){
        if (status == WL_SCAN_COMPLETED || status == WL_DISCONNECTED || status == WL_IDLE_STATUS){
            //wait more for connecting
            return;
        }
        else{
            isConnecting_ = false;

            Serial.println(F("Wifi connection failed."));
            Serial.print(F("Wifi status: "));
            Serial.println(WiFi.status());
            return;
        }
    }

    if(isConnected_){
        isConnected_ = false;
        WiFi.disconnect();
    }

    auto now = millis();
    auto diff = now - lastWiFiBeginTime_;

    if(isFirstTrial_ || diff > 30000){

        Serial.print(F("Connecting to "));
        Serial.println(WIFI_SSID);
        Serial.println("");

        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        isConnected_ = false;
        isConnecting_ = true;
        isFirstTrial_ = false;

        lastWiFiBeginTime_ = now;
    }

}

void WifiConnection::begin(){
    isBegun_ = true;
    WiFi.mode(WIFI_STA);
}


void WifiConnection::end(){
    isBegun_ = false;
    isConnecting_ = false;
    isConnected_ = false;
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);

}