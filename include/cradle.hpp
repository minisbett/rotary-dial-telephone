#pragma once
#include "Arduino.h"

enum CradleState
{
    HUNG_UP = 0,
    PICKED_UP = 1
};

typedef void (*StateChangedCallback)(CradleState state);

inline class Cradle
{
public:
    void begin(StateChangedCallback stateChanged);
    void loop();
    CradleState read();
    
    CradleState state = HUNG_UP;

private:
    StateChangedCallback onStateChanged = NULL;
} Cradle;