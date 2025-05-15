#pragma once
#include <Arduino.h>
#include <SoftwareSerial.h>
#include "definitions.hpp"

class GSMShield
{
public:
    void begin();
    void call(String phoneNumber);
    void accept();
    void hangUp();
    bool isReadAvailable();
    String read();

private:
    SoftwareSerial gsmSerial = SoftwareSerial(GSM_RX, GSM_TX);
} GSMShield;