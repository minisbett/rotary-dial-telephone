#include <Arduino.h>
#include "phone.hpp"

void setup()
{
    Phone.begin();

    // Enable builtin-led at minimal brightness to indicate the pico power state.
    pinMode(25, OUTPUT);
    analogWrite(25, 10);
}

void loop()
{
    Phone.loop();
}