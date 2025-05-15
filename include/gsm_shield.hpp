#pragma once
#include <Arduino.h>
#include <SoftwareSerial.h>
#include "definitions.hpp"

inline class GSMShield
{
public:
    void begin();
    void call(String phoneNumber);
    void accept();
    void hangUp();
    bool isReadAvailable();
    String read();
    void write(String str);

private:
} GSMShield;