#include <Arduino.h>
#include "gsm_shield.hpp"
#include "phone.hpp"
#include "definitions.hpp"

void Phone::begin()
{
    Serial.begin(19200);
    GSMShield.begin();

    pinMode(PIN_CRADLE, INPUT_PULLUP);
    pinMode(PIN_ROTARY_DIAL_STATE, INPUT_PULLUP);
    pinMode(PIN_ROTARY_DIAL_PULSE, INPUT_PULLUP);
    pinMode(PIN_RING_STATE, OUTPUT);
    pinMode(PIN_RING_PULSE, OUTPUT);
}

void Phone::loop()
{
    processGsmInput();
    processRing();
    processCradle();
    processRotaryDial();
    checkForAutoCall();
}

void Phone::processRing()
{
    if (GSMShield.isReadAvailable())
    {
        String str = GSMShield.read();
        if (str.indexOf("RING") != -1)
        {
            digitalWrite(PIN_RING_STATE, HIGH);
            for (int i = 0; i < 40; i++)
            {
                digitalWrite(PIN_RING_PULSE, HIGH);
                delay(40);
                digitalWrite(PIN_RING_PULSE, LOW);
                delay(40);
            }

            digitalWrite(PIN_RING_STATE, LOW);
        }
    }
}

void Phone::processCradle()
{
    bool newCradleState = digitalRead(PIN_CRADLE);

    if (newCradleState != cradleState)
    {
        delay(50); // debounce
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
    if (cradleState == HIGH)
        return;

    int dialCounter = 0;
    bool hasDialed = false;

    while (digitalRead(PIN_ROTARY_DIAL_STATE) == LOW)
    {
        lastNumberDialTime = millis();
        hasDialed = true;

        if (digitalRead(PIN_ROTARY_DIAL_PULSE) == HIGH)
        {
            dialCounter++;
            delay(ROTARY_DIAL_PULSE_RATE);
            Serial.println("[phone] Dial Counter: " + String(dialCounter));
        }
    }

    if (hasDialed && dialCounter > 0)
    {
        dialCounter = dialCounter == 10 ? 0 : dialCounter;
        phoneNumber += String(dialCounter);
        Serial.println("[phone] Current Number: " + phoneNumber);
    }
}

void Phone::checkForAutoCall()
{
    if (cradleState == HIGH || isCalling || phoneNumber.length() > 5)
        return;

    unsigned long delta = millis() - lastNumberDialTime;
    if (delta % 1000 == 0)
    {
        Serial.println("[phone] Auto Call in " + String((AUTO_CALL_TIMEOUT - delta) / 1000) + "s");
    }
    if (delta >= AUTO_CALL_TIMEOUT)
    {
        isCalling = true;
        GSMShield.call(phoneNumber);
    }
}

void Phone::processGsmInput()
{
    while (Serial.available())
    {
        String str = Serial.readStringUntil('\n');
        GSMShield.write(str);
    }
}