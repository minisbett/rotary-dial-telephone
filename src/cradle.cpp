#include "Arduino.h"
#include "cradle.hpp"
#include "definitions.hpp"

void Cradle::begin(StateChangedCallback stateChanged)
{
    onStateChanged = stateChanged;

    pinMode(PIN_CRADLE, INPUT_PULLUP);
}

PinStatus Cradle::read()
{
    return digitalRead(PIN_CRADLE);
}

void Cradle::loop()
{
    PinStatus newState = read();
    if (state != newState)
    {
        state = newState;
        onStateChanged(state);

#if DEBUG_CRADLE
        Serial.println("[cradle/debug] state = " + String(state == CRADLE_PICKED_UP ? "PICKED_UP" : "HUNG_UP"));
#endif
    }
}