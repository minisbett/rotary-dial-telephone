#include "gsm_shield.hpp"
#include <Arduino.h>

void GSMShield::begin()
{
    gsmSerial.begin(19200);
}

void GSMShield::call(String phoneNumber)
{
    gsmSerial.println("ATD " + phoneNumber + ";");
    Serial.println("[gsmSerial] call(" + phoneNumber + ");");
}

void GSMShield::accept()
{
    gsmSerial.println("ATA");
    Serial.println("[gsmSerial] accept");
}

void GSMShield::hangUp()
{
    gsmSerial.println("ATH");
    Serial.println("[gsmSerial] hangUp");
}

bool GSMShield::isReadAvailable()
{
    return gsmSerial.available();
}

String GSMShield::read()
{
    String str = gsmSerial.readString();
    Serial.println("[gsmSerial] read: " + str);
    return str;
}