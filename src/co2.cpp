#include "co2.h"
#include "config.h"

#include <M5Atom.h>

#define HIGH_FREQUENNCY_COUNT (36)
#define LOW_FREQUENNCY_COUNT (480)
#define HISTORICAL_DATA_MAX_COUNT (LOW_FREQUENNCY_COUNT + HIGH_FREQUENNCY_COUNT) // more than 1 day CO2 logs


CO2::CO2(){}

void CO2::begin(){

    Serial2.begin(9600, SERIAL_8N1, CO2_RXPIN, CO2_TXPIN);
    mhz_.begin(Serial2);

    if(CO2_SENSOR_AUTO_CALIBRATION){
        mhz_.autoCalibration(true);
    }
    else{
        mhz_.autoCalibration(false);
    }

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

    auto rawCo2 = mhz_.getCO2();

    if(stableCount < 5){

        int diff = lastValue_ - rawCo2;
        lastValue_ = rawCo2;

        if(abs(diff) > 50){
            Serial.print(F("Waiting for CO2 sensor to stabilize... "));
            Serial.print(rawCo2);
            Serial.println(F("ppm"));

            stableCount = 0;
        }
        else{
            stableCount++;
        }

        return;
    }

    lastValue_ = rawCo2;

    rawCo2Sum_ += rawCo2;
    rawCo2SumCount_ ++;

    auto rawTemperature = mhz_.getTemperature(true) + temperatureCalibrationValue_;
    rawTemperatureSum_ += rawTemperature;


    if(diffTimeFromLastCO2 > 5000){

        auto co2 = rawCo2Sum_ / rawCo2SumCount_;
        auto temperature = rawTemperatureSum_ / rawCo2SumCount_;
        rawCo2Sum_ = 0;
        rawTemperatureSum_ = 0;
        rawCo2SumCount_ =  0;
        
        lastCO2Time_ = now;

        sensorGotValue(co2);

        auto totalSize = nearCo2Histories_.size() + farCo2Histories_.size();

        nearCo2Histories_.emplace_back(now, co2, temperature);

        if(nearCo2Histories_.size() > HIGH_FREQUENNCY_COUNT && totalSize > HISTORICAL_DATA_MAX_COUNT){

            int co2Sum = 0;
            float temperatureSum = 0;
            unsigned long long timeSum = 0;

            for(int i=0;i<HIGH_FREQUENNCY_COUNT;i++){
                auto e = nearCo2Histories_.front();
                co2Sum  += e.co2();
                temperatureSum += e.temperature();
                timeSum += e.time();
                nearCo2Histories_.pop_front();
            }

            farCo2Histories_.emplace_back(timeSum/HIGH_FREQUENNCY_COUNT, co2Sum/HIGH_FREQUENNCY_COUNT, temperatureSum/HIGH_FREQUENNCY_COUNT);
        }
        

        if(farCo2Histories_.size() > LOW_FREQUENNCY_COUNT){
            farCo2Histories_.pop_front();
        }

        Serial.print(F("CO2 (ppm): "));
        Serial.println(co2);

        Serial.print(F("Temperature (C): "));
        Serial.println(temperature);
    }

}


std::list<CO2History>* CO2::farCo2Histories(){
    return &farCo2Histories_;
}

std::list<CO2History>* CO2::nearCo2Histories(){
    return &nearCo2Histories_;
}


void CO2::calibrate(){
    mhz_.calibrate();
}


void CO2::clearHistories(){
    nearCo2Histories_.clear();
    farCo2Histories_.clear();
}