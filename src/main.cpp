#include <Arduino.h>
#include "phone.hpp"

void setup()
{
    Phone.begin();

    pinMode(25, OUTPUT);
    analogWrite(25, 6);
}

void loop()
{
    Phone.loop();
}