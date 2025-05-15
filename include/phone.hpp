#pragma once
#include "gsm_shield.hpp"

class Phone
{
public:
    void begin();
    void loop();
private:
    void processGSMRead();
    void processCradle();
    void processRotaryDial();
    void checkForAutoCall();

    String phoneNumber = "";
    bool cradleState = 0;
    bool isCalling = false;
    unsigned long lastNumberDialTime = 0;
} Phone;