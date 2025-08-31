#pragma once
#include <Arduino.h>

inline class Caller
{
public:
    void begin();
    void loop();
private:
    bool isPhoneNumberEligible();
} Caller;