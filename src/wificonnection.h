#pragma once

class WifiConnection{

public:
    void tick();
    void begin(); 
    void end();
    bool isConnected() const{
        return isConnected_;
    }

private:
    bool isBegun_ = false;
    bool isConnected_ = false;
    bool isConnecting_ = false;
    unsigned long lastWiFiBeginTime_ = 0;
    unsigned long lastPingTime_ = 0;
    bool isFirstTrial_  = true;


};