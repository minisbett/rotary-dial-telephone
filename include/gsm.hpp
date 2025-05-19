#pragma once
#include <Arduino.h>

typedef void (*RingCallback)();

inline class GSM
{
public:
    void begin(RingCallback ring);
    void call(String phoneNumber);
    void accept();
    void hangUp();
    void loop();

    bool isCalling = false;

private:
    bool isReadAvailable();
    String readLine();
    bool readOkOrError();
    void write(String str);

    RingCallback onRing;
} GSM;