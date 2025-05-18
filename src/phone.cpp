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

    // Use 425hz analog for the dialing idle sound signal
    analogWriteFreq(425);
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
    while (GSMShield.isReadAvailable())
    {
        String str = GSMShield.readLine();
        if (str == "RING")
        {
            digitalWrite(PIN_RING_STATE, HIGH);
            for (int i = 0; i < 40; i++)
            {
                // If the phone was picked up or hung up, immediately abort the ringing.
                if (cradleState != digitalRead(PIN_CRADLE))
                    break;

                digitalWrite(PIN_RING_PULSE, HIGH);
                delay(20);
                digitalWrite(PIN_RING_PULSE, LOW);
                delay(20);
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
        delay(50); // A debounce to not trigger multiple times on power fluctuation
        cradleState = newCradleState;

        if (cradleState == LOW) // Phone was hung up
        {
            GSMShield.hangUp();

            // The phone number buffer is reset when hanging to allow cancelling a dial.
            phoneNumber = "";

            // The isCalling state is set to true when callng a number (ATD), and regardless of
            // whether the opposite hang up is true until this code (once this phone hang up)
            isCalling = false;
        }
        else if (newCradleState == HIGH) // Phone was picked up
        {
            GSMShield.accept();
        }
    }

    // While the phone is picked up and the phone not in a call, play back the dialing idle sound.
    if (cradleState == HIGH && !isCalling)
    {
        analogWrite(PIN_DIAL_IDLE_SOUND, 255);
    }
}

void Phone::processRotaryDial()
{
    if (cradleState == HIGH) // Only while phone is picked up
        return;

    int dialCounter = 0;
    while (digitalRead(PIN_ROTARY_DIAL_STATE) == LOW)
    {
        lastNumberDialTime = millis();

        if (digitalRead(PIN_ROTARY_DIAL_PULSE) == HIGH)
        {
            dialCounter++;
            delay(ROTARY_DIAL_PULSE_RATE);
            Serial.println("[phone] Dial Counter: " + String(dialCounter));
        }
    }

    if (dialCounter > 0)
    {
        dialCounter = dialCounter == 10 ? 0 : dialCounter;
        phoneNumber += String(dialCounter);
        Serial.println("[phone] Current Number: " + phoneNumber);
    }
}

void Phone::checkForAutoCall()
{
    // Only perform an auto-call if the phone is picked up, not in an already calling state
    // and the phone number at least 6 numbers long. Due to isCalling being reset when hanging
    // the phone up, not when the call actually ends, this prevents auto-calling again when
    // the opposite hang up, and expects the user to hang up their phone before that.
    if (cradleState == LOW || isCalling || phoneNumber.length() <= 5)
        return;

    unsigned long delta = millis() - lastNumberDialTime;
    if (delta % 1000 == 0)
    {
        Serial.println("[phone] Auto Call in " + String((AUTO_CALL_TIMEOUT - delta) / 1000) + "s");
    }
    if (delta >= AUTO_CALL_TIMEOUT)
    {
        GSMShield.call(phoneNumber);

        // isCalling is set to true to remember that a call is now on-going. This state is not
        // reset when the opposite hangs up the phone, and only when the user hangs up theirs.
        // This is used in order to not automatically auto-call again when the call ended.
        isCalling = true;
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