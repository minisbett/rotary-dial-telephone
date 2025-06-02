#include "Arduino.h"
#include "cradle.hpp"
#include "definitions.hpp"

void Cradle::begin(StateChangedCallback stateChanged)
{
    onStateChanged = stateChanged;

    pinMode(PIN_CRADLE, INPUT_PULLUP);
}

CradleState Cradle::read()
{
    return (CradleState)digitalRead(PIN_CRADLE);
}

void Cradle::loop()
{
    CradleState newState = read();
    if (state != newState)
    {
        state = newState;
        onStateChanged(state);

#if DEBUG_CRADLE
        Serial.println("[cradle/debug] state = " + String(state ? "PICKED_UP" : "HUNG_UP"));
#endif
    }
}