#include "co2.h"
#include "config.h"

#include <M5Atom.h>

#define HISTORICAL_DATA_MAX_COUNT (500) // more than 1 day CO2 logs

CO2::CO2(){}

void CO2::begin(){

    Serial2.begin(9600, SERIAL_8N1, CO2_RXPIN, CO2_TXPIN);
    mhz_.begin(Serial2);
    mhz_.autoCalibration(false);

    char version[5] = {0,0,0,0,0};

    mhz_.getVersion(version);
    Serial.print(F("CO2 sensor version: "));
    Serial.println(version);

    rotatedDisplay_.begin();

}

void CO2::sensorGotValue(int ppm){

    int ppm100 = round(ppm / 100);

    if(ppm100 <= 7){
        rotatedDisplay_.drawNumber(ppm100, CRGB::Green);
    }
    else if(ppm100 <= 9){
        rotatedDisplay_.drawNumber(ppm100, CRGB::Yellow);
    }
    else{
        rotatedDisplay_.drawNumber(ppm100, CRGB::Red);
    }

}

void CO2::tick(){

    auto now = millis();
    auto diffTimeFromLastCO2 = now - lastCO2Time_;

    if(diffTimeFromLastCO2 > 1000){

        auto co2 = mhz_.getCO2();
        lastCO2Time_ = now;

        auto temperature = mhz_.getTemperature() + TEMPERATURE_CALIBRATION_VALUE;

        if(stableCount < 5){

            int diff = lastValue_ - co2;
            lastValue_ = co2;

            if(abs(diff) > 50){
                Serial.print(F("Waiting for CO2 sensor to stabilize... "));
                Serial.print(co2);
                Serial.println(F("ppm"));

                stableCount = 0;
            }
            else{
                stableCount++;
            }

            return;
        }

        lastValue_ = co2;


        sensorGotValue(co2);

        auto diffTimeFromLastLog = now - lastLogTime_;

        //For first 30minutes, Log interval = 5 seconds
        auto logInterval = 5000;
        
        //After 30minutes, Log interval = 3 minutes
        if(co2Histories_.size() > 12*30){ 
            logInterval = 180000;
        }

        if(diffTimeFromLastLog  >= logInterval){

            co2Histories_.emplace_back(now, co2, temperature);

            if(co2Histories_.size() > HISTORICAL_DATA_MAX_COUNT){
                co2Histories_.pop_front();
            }

            lastLogTime_ = now;
        }

        Serial.print(F("CO2 (ppm): "));
        Serial.println(co2);

        Serial.print(F("Temperature (C): "));
        Serial.println(temperature);
    }

}


void CO2::calibrate(){
    mhz_.calibrate();
}