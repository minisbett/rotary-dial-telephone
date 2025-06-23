#pragma once
#include "Arduino.h"
#include <definitions.hpp>

#define CRADLE_PICKED_UP LOW
#define CRADLE_HUNG_UP HIGH

typedef void (*StateChangedCallback)(PinStatus state);

inline class Cradle
{
public:
    void begin(StateChangedCallback stateChanged);
    void loop();
    PinStatus read();
    
    PinStatus state = CRADLE_HUNG_UP;

private:
    StateChangedCallback onStateChanged = NULL;
} Cradle;