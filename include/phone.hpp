#pragma once
#include "gsm_shield.hpp"

inline class Phone
{
public:
    void begin();
    void loop();
private:
    void processRing();
    void processCradle();
    void processRotaryDial();
    void checkForAutoCall();
    void processGsmInput();

    String phoneNumber = "";
    bool cradleState = 0;
    bool isCalling = false;
    unsigned long lastNumberDialTime = 0;
} Phone;