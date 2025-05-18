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
    write("ATD " + phoneNumber + ";");
}

void GSMShield::accept()
{
    write("ATA");
}

void GSMShield::hangUp()
{
    write("ATH");
}

bool GSMShield::isReadAvailable()
{
    return Serial1.available();
}

String GSMShield::readLine()
{
    String str = Serial1.readStringUntil('\r');
    str.replace("\n", "");
    if (str != "")
        Serial.println("[gsm] # " + str);

    return str;
}

void GSMShield::write(String str)
{
    Serial1.println(str);
    Serial.println("[gsm] @ " + str);
}