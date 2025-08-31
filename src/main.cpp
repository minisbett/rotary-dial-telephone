#include <Arduino.h>
#include "gsm.hpp"
#include "cradle.hpp"
#include "rotary_dial.hpp"
#include "caller.hpp"

void onCradleStateChanged(PinStatus state)
{
    if (state == CRADLE_HUNG_UP)
    {
        GSM.hangUp();
        RotaryDial.number = "";
    }
    else
    {
        GSM.accept();
    }
}

void onRing()
{
    if(Cradle.state == CRADLE_PICKED_UP)
    {
        GSM.hangUp();
        return;
    }

    pinMode(PIN_RING_STATE, OUTPUT);
    pinMode(PIN_RING_PULSE, OUTPUT);
    digitalWrite(PIN_RING_STATE, HIGH);

    for (int i = 0; i < 40; i++)
    {
        if (Cradle.read() == CRADLE_PICKED_UP)
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

void setSpeakerRelais()
{
    pinMode(PIN_SPEAKER_RELAIS_SET, OUTPUT);
    digitalWrite(PIN_SPEAKER_RELAIS_SET, HIGH);
    delay(20);
    digitalWrite(PIN_SPEAKER_RELAIS_SET, LOW);
}

void resetSpeakerRelais()
{
    pinMode(PIN_SPEAKER_RELAIS_RESET, OUTPUT);
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

    Serial.begin(19200);
    GSM.begin(onRing);
    Cradle.begin(onCradleStateChanged);
    RotaryDial.begin();
    Caller.begin();

    tone(PIN_IDLE_BEEP, 425);
}

void loop()
{
    GSM.loop();
    Cradle.loop();

    if (Cradle.state == CRADLE_PICKED_UP)
    {
        RotaryDial.loop();
        Caller.loop();

        if (!GSM.isCalling && !isIdleBeeping)
        {
            isIdleBeeping = true;
            resetSpeakerRelais();
        }
    }

    if (isIdleBeeping && (Cradle.state == CRADLE_HUNG_UP || GSM.isCalling))
    {
        isIdleBeeping = false;
        setSpeakerRelais();
    }
}