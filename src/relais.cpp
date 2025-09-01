#include <Arduino.h>
#include "relais.hpp"
#include "definitions.hpp"

void Relais::set()
{
    pinMode(PIN_SPEAKER_RELAIS_SET, OUTPUT);
    digitalWrite(PIN_SPEAKER_RELAIS_SET, HIGH);
    delay(20);
    digitalWrite(PIN_SPEAKER_RELAIS_SET, LOW);
}

void Relais::reset()
{
    pinMode(PIN_SPEAKER_RELAIS_RESET, OUTPUT);
    digitalWrite(PIN_SPEAKER_RELAIS_RESET, HIGH);
    delay(20);
    digitalWrite(PIN_SPEAKER_RELAIS_RESET, LOW);
}