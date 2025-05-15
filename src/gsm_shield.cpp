#include "gsm_shield.hpp"
#include <Arduino.h>

void GSMShield::begin()
{
    Serial1.setTX(PIN_GSM_TX);
    Serial1.setRX(PIN_GSM_RX);
    Serial1.begin(19200);
}

void GSMShield::call(String phoneNumber)
{
    Serial1.println("ATD " + phoneNumber + ";");
    Serial.println("[gsmSerial] -> ATD " + phoneNumber + ";");
}

void GSMShield::accept()
{
    Serial1.println("ATA");
    Serial.println("[gsmSerial] -> ATA");
}

void GSMShield::hangUp()
{
    Serial1.println("ATH");
    Serial.println("[gsmSerial] -> ATH");
}

bool GSMShield::isReadAvailable()
{
    return Serial1.available();
}

String GSMShield::read()
{
    String str = Serial1.readString();
    Serial.println("[gsmSerial] <- " + str);
    return str;
}

void GSMShield::write(String str)
{
    Serial1.println(str);
}