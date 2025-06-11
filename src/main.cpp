#include <Arduino.h>
#include "gsm.hpp"
#include "cradle.hpp"
#include "rotary_dial.hpp"
#include "definitions.hpp"

void onCradleStateChanged(CradleState state)
{
    if (state == HUNG_UP)
    {
        GSM.hangUp();
        RotaryDial.phoneNumber = "";
    }
    else
    {
        GSM.accept();
    }
}

void onRing()
{
    pinMode(PIN_RING_STATE, OUTPUT);
    pinMode(PIN_RING_PULSE, OUTPUT);
    digitalWrite(PIN_RING_STATE, HIGH);

    for (int i = 0; i < 40; i++)
    {
        if (Cradle.state != Cradle.read())
        {
            break;
        }

        digitalWrite(PIN_RING_PULSE, HIGH);
        delay(20);
        digitalWrite(PIN_RING_PULSE, LOW);
        delay(20);
    }

    digitalWrite(PIN_RING_STATE, LOW);
}

void checkAutoCall()
{
    if (!GSM.isCalling && RotaryDial.phoneNumber.length() >= AUTO_CALL_MIN_NUMBER_LENGTH)
    {
        unsigned long delta = millis() - RotaryDial.lastDialTimeMs;

        if (delta >= AUTO_CALL_TIMEOUT)
        {
            GSM.call(RotaryDial.phoneNumber);
        }

#if DEBUG_AUTO_CALL
        if (delta % 500 == 0)
        {
            String remaining = String(round((AUTO_CALL_TIMEOUT - delta) / 100.0) / 10);
            Serial.println("[phone/debug] Auto Call in " + remaining + "s");
        }
#endif
    }
}

void setSpeakerRelais()
{
    digitalWrite(PIN_SPEAKER_RELAIS_SET, HIGH);
    delay(20);
    digitalWrite(PIN_SPEAKER_RELAIS_SET, LOW);
}

void resetSpeakerRelais()
{
    digitalWrite(PIN_SPEAKER_RELAIS_RESET, HIGH);
    delay(20);
    digitalWrite(PIN_SPEAKER_RELAIS_RESET, LOW);
}

bool isIdleBeeping = false;

void setup()
{
    // Enable builtin-led at minimal brightness to indicate the pico power state.
    pinMode(25, OUTPUT);
    analogWrite(25, 10);

    GSM.begin(onRing);
    Cradle.begin(onCradleStateChanged);
    RotaryDial.begin();
}

void loop()
{
    GSM.loop();
    Cradle.loop();

    if (Cradle.state == PICKED_UP)
    {
        RotaryDial.loop();
        checkAutoCall();

        if (!GSM.isCalling && !isIdleBeeping)
        {
            isIdleBeeping = true;
            setSpeakerRelais();
        }
    }

    if (isIdleBeeping && (Cradle.state == HUNG_UP || GSM.isCalling))
    {
        isIdleBeeping = false;
        resetSpeakerRelais();
    }
}