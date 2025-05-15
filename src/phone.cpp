#include <Arduino.h>
#include "gsm_shield.hpp"
#include "phone.hpp"
#include "definitions.hpp"

void Phone::begin()
{
    Serial.begin(19200);
    GSMShield.begin();
}

void Phone::loop()
{
    processGSMRead();
    processCradle();

    if (cradleState == LOW)
    {
        processRotaryDial();

        if (!isCalling)
            checkForAutoCall();
    }
}

void Phone::processGSMRead()
{
    if (GSMShield.isReadAvailable())
    {
        String str = GSMShield.read();
    }
}

void Phone::processCradle()
{
    bool newCradleState = digitalRead(PIN_CRADLE);

    if (newCradleState != cradleState)
    {
        cradleState = newCradleState;

        if (cradleState == HIGH)
        {
            GSMShield.hangUp();
            phoneNumber = "";
            isCalling = false;
        }
        else
        {
            GSMShield.accept();
        }
    }
}

void Phone::processRotaryDial()
{
    int dialCounter = 0;
    bool hasDialed = false;

    while (digitalRead(PIN_ROTARY_DIAL_STATE) == HIGH)
    {
        lastNumberDialTime = millis();
        hasDialed = true;

        if (digitalRead(PIN_ROTARY_DIAL_COUNTER) == HIGH)
        {
            dialCounter++;
            delay(70);
            Serial.println("[phone] Dial Counter: " + String(dialCounter));
        }
    }

    if (hasDialed && dialCounter > 0)
    {
        dialCounter = dialCounter == 10 ? 0 : dialCounter;
        phoneNumber += String(dialCounter);
        Serial.println("[phone] Current Dial: " + phoneNumber);
    }
}

void Phone::checkForAutoCall()
{
    unsigned long delta = millis() - lastNumberDialTime;
    if(delta % 500 == 0)
    {
        Serial.println("[phone] Auto Call Timer: " + String(AUTO_CALL_TIMEOUT - delta));
    }
    if (phoneNumber.length() > 0 && delta >= AUTO_CALL_TIMEOUT)
    {
        isCalling = true;
        GSMShield.call(phoneNumber);
    }
}