#pragma once
#include <Arduino.h>

inline class RotaryDial
{
public:
    void begin();
    void loop();

    String phoneNumber = "";
    unsigned long lastDialTimeMs = 0;
private:
} RotaryDial;