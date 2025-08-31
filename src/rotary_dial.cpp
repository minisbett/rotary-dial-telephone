#include <Arduino.h>
#include "rotary_dial.hpp"
#include "definitions.hpp"

void RotaryDial::begin()
{
    pinMode(PIN_ROTARY_DIAL_STATE, INPUT_PULLUP);
    pinMode(PIN_ROTARY_DIAL_PULSE, INPUT_PULLUP);
}

void RotaryDial::loop()
{
    int counter = 0;
    while (digitalRead(PIN_ROTARY_DIAL_STATE) == LOW)
    {
        if (digitalRead(PIN_ROTARY_DIAL_PULSE) == HIGH)
        {
            counter++;
            lastDialTimeMs = millis();

#if DEBUG_ROTARY_DIAL
            Serial.println("[rotarydial/debug] counter = " + String(counter));
#endif

            delay(1000.0 / ROTARY_DIAL_PULSE_FREQ);
        }
    }

    if (counter > 0)
    {
        counter %= 10; // 10 -> 0
        number += String(counter);
        Serial.println("[rotarydial] Number: " + number);
    }
}