#pragma once

#include  "rotateddisplay.h"

#include <MHZ19.h>

#include <list>


class CO2History{
public:
    explicit CO2History(): time_(0), co2_(0), temperature_(0){}
    explicit CO2History(unsigned long time, int co2, float temperature): time_(time), co2_(co2), temperature_(temperature){}

    unsigned long time() const {return time_;}
    int co2() const {return co2_;}
    float temperature() const {return temperature_;}

private:
    unsigned long time_;
    int co2_;
    float temperature_;

};


class CO2{

public:
    explicit CO2();
    void begin();
    void tick();
    void calibrate();
    void setTemperatureCalibrationValue(float v){
        temperatureCalibrationValue_ = v;
    }
    void clearHistories();

    void sensorGotValue(int ppm);
    std::list<CO2History>* farCo2Histories();
    std::list<CO2History>* nearCo2Histories();
    
private:
    MHZ19 mhz_; 
    RotatedDisplay rotatedDisplay_;

    std::list<CO2History> nearCo2Histories_;
    std::list<CO2History> farCo2Histories_;

    int rawCo2Sum_ = 0;
    int rawCo2SumCount_ = 0;
    float rawTemperatureSum_ = 0;

    bool isLEDBlinkingOnTime_ = false;
    unsigned long  lastCO2Time_ = 0;
    int stableCount = 0;
    int lastValue_ = INT_MAX;
    float temperatureCalibrationValue_ = 0;
};