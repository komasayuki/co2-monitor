#include "co2.h"
#include "config.h"

#include <M5Atom.h>


CO2::CO2(){}

void CO2::begin(){

    Serial2.begin(9600, SERIAL_8N1, CO2_RXPIN, CO2_TXPIN);
    mhz_.begin(Serial2);
    mhz_.autoCalibration(false);

    char version[5] = {0,0,0,0,0};

    mhz_.getVersion(version);
    Serial.print("CO2 sensor version: ");
    Serial.println(version);

    rotatedDisplay_.begin();

}

void CO2::sensorGotValue(int ppm){

    if(!isStabled_){
        int diff = lastValue_ - ppm;
        if(diff > 100){
            //wait for CO2 sensor to stabilize
            lastValue_ = ppm;
            return;
        }
        isStabled_ = true;
    }

    lastValue_ = ppm;

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
        auto temperature = mhz_.getTemperature() + TEMPERATURE_CALIBRATION_VALUE;

        sensorGotValue(co2);

        auto diffTimeFromLastLog = now - lastLogTime_;

        auto logInterval = 5000;
        if(co2Histories_.size() > 12*60){
            logInterval = 60000;
        }

        if(diffTimeFromLastLog  >= logInterval){

            co2Histories_.emplace_back(now, co2, temperature);

            if(co2Histories_.size() > HISTORICAL_DATA_MAX_COUNT){
                co2Histories_.pop_front();
            }

            lastLogTime_ = now;
        }

        Serial.print("CO2 (ppm): ");
        Serial.println(co2);

        Serial.print("Temperature (C): ");
        Serial.println(temperature);

        lastCO2Time_ = now;
    }

}


void CO2::calibrate(){
    mhz_.calibrate();
}