#include <Arduino.h>
#include <EEPROM.h>
#include "caller.hpp"
#include "rotary_dial.hpp"
#include "gsm.hpp"
#include "definitions.hpp"

#define EEPROM_NUMBER_BUFFER_SIZE 15

void Caller::begin()
{
    EEPROM.begin(256);
}

void Caller::loop()
{
    if (!GSM.isCalling && isPhoneNumberEligible())
    {
        unsigned long delta = millis() - RotaryDial.lastDialTimeMs;

#if DEBUG_CALLER
        if (delta % 500 == 0)
        {
            String remaining = String(round((AUTO_CALL_TIMEOUT - delta) / 100.0) / 10);
            Serial.println("[caller/debug] Call in " + remaining + "s");
        }
#endif

        if (delta < AUTO_CALL_TIMEOUT)
            return;

        if (RotaryDial.number.startsWith("000"))
        {
            uint8_t slot = RotaryDial.number.substring(3, 4).toInt();
            String number = RotaryDial.number.substring(4);

            char numberBuffer[EEPROM_NUMBER_BUFFER_SIZE];
            number.toCharArray(numberBuffer, sizeof(numberBuffer));
            EEPROM.put((slot - 1) * EEPROM_NUMBER_BUFFER_SIZE, numberBuffer);
            EEPROM.commit();

            RotaryDial.number = "";
#if DEBUG_CALLER
            Serial.println("[caller/debug] Stored \"" + String(numberBuffer) + "\" in slot " + String(slot));
#endif
        }
        else if (RotaryDial.number.startsWith("00"))
        {
            uint8_t slot = RotaryDial.number.substring(2, 3).toInt();
            char numberBuffer[EEPROM_NUMBER_BUFFER_SIZE];
            EEPROM.get((slot - 1) * EEPROM_NUMBER_BUFFER_SIZE, numberBuffer);
            String number = String(numberBuffer);

            GSM.call(number);

#if DEBUG_CALLER
            Serial.println("[caller/debug] Called \"" + number + "\" from slot " + String(slot));
#endif
        }
        else
            GSM.call(RotaryDial.number);
    }
}

bool Caller::isPhoneNumberEligible()
{
    String number = RotaryDial.number;

    // real number is called (not a 000 save dial), and min. configured length
    if (number.length() >= AUTO_CALL_MIN_NUMBER_LENGTH && !RotaryDial.number.startsWith("000"))
        return true;

    // quick-dial (not save dial) is performed, and min. "00" + a slot number
    if (number.startsWith("00") && !number.startsWith("000") && number.length() >= 3)
        return true;

    // save dial is performed, and min. "000" + slot number + the usual min length
    if (number.startsWith("000") && number.length() >= 4 + AUTO_CALL_MIN_NUMBER_LENGTH)
        return true;

    return false;
}