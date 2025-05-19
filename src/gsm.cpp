#include "gsm.hpp"
#include "definitions.hpp"

void GSM::begin(RingCallback ring)
{
    onRing = ring;

    Serial1.begin(19200);
}

void GSM::call(String phoneNumber)
{
    write("ATD " + phoneNumber + ";");

    if (readOkOrError())
    {
        isCalling = true;

#if DEBUG_GSM
        Serial.println("[gsm/debug] isCalling = true (call initiated)");
#endif
    }
}

void GSM::accept()
{
    write("ATA");

    if (readOkOrError())
    {
        isCalling = true;

#if DEBUG_GSM
        Serial.println("[gsm/debug] isCalling = true (call accepted)");
#endif
    }
}

void GSM::hangUp()
{
    write("ATH");

    isCalling = false;

#if DEBUG_GSM
    Serial.println("[gsm/debug] isCalling = false (hung up)");
#endif
}

void GSM::loop()
{
    while (isReadAvailable())
    {
        String str = readLine();
        if (str == "NO CARRIER" || str == "BUSY" || str == "NO ANSWER")
        {
            isCalling = false;

#if DEBUG_GSM
            Serial.println("[gsm/debug] isCalling = false (" + str + ")");
#endif
        }
        else if (str == "RING")
        {
            onRing();

#if DEBUG_GSM
            Serial.println("[gsm/debug] onRing executed");
#endif
        }
    }

    while (Serial.available())
    {
        String str = Serial.readStringUntil('\r');
#if DEBUG_GSM
        if (str == "accept")
        {
            accept();
            return;
        }
        else if (str == "hangUp")
        {
            hangUp();
            return;
        }
        else if (str.startsWith("call "))
        {
            call(str.substring(5));
            return;
        }
#endif

        write(str);
    }
}

bool GSM::isReadAvailable()
{
    return Serial1.available();
}

String GSM::readLine()
{
    String str = Serial1.readStringUntil('\r');
    str.replace("\n", "");
    if (str != "")
    {
        Serial.println("[gsm] # " + str);
    }

    return str;
}

bool GSM::readOkOrError()
{
    int time = millis();
    while (millis() < time + 1000) // 1 second timeout
    {
        String str = readLine();
        if (str == "OK")
        {
            return true;
        }
        else if (str == "ERROR")
        {
            return false;
        }
    }

    return false;
}

void GSM::write(String str)
{
    Serial1.print(str + "\r");
    Serial.println("[gsm] @ " + str);
}