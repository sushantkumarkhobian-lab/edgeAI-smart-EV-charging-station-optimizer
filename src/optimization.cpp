#include <Arduino.h>

#include "optimization.h"
#include "config.h"
#include "State.h"

void runOptimization()
{
    float totalStationPower = power;

    overloadActive = false;

    if (bayStatus != "CHARGING")
    {
        bool peakHour =
            (lastHourOfDay >= peakTariffStartHr &&
             lastHourOfDay <= peakTariffEndHr);

        if (peakHour && predictedArrivalProb >= predictionThreshold)
        {
            loadDecision = "STANDBY_EXPECTING";
        }
        else if (peakHour)
        {
            loadDecision = "LOW_DEMAND";
        }

        return;
    }

    if (bayStatus == "CHARGING")
    {
        if (current > overloadCurrentA)
        {
            loadDecision = "THROTTLE";
            throttleLevel = 50;
            overloadActive = true;

            Serial.println("!! Overcurrent -> THROTTLE @ 50%.");
        }
        else if (totalStationPower > maxStationLoadW)
        {
            if (predictedDurationMin > 10)
            {
                loadDecision = "THROTTLE";
                throttleLevel = 70;

                Serial.println(
                    "!! Station power cap exceeded -> THROTTLE @ 70%."
                );
            }
            else
            {
                loadDecision = "ALLOW";
                throttleLevel = 100;
            }
        }
        else
        {
            loadDecision = "ALLOW";
            throttleLevel = 100;
        }
    }
}

void applyRelayDutyCycle()
{
    if (bayStatus != "CHARGING")
    {
        digitalWrite(RELAY_PIN, LOW);
        return;
    }

    if (throttleLevel >= 100)
    {
        digitalWrite(RELAY_PIN, HIGH);
        return;
    }

    if (throttleLevel <= 0)
    {
        digitalWrite(RELAY_PIN, LOW);
        return;
    }

    unsigned long phase =
        millis() % DUTY_CYCLE_WINDOW_MS;

    unsigned long onTime =
        (DUTY_CYCLE_WINDOW_MS * throttleLevel) / 100;

    digitalWrite(
        RELAY_PIN,
        phase < onTime ? HIGH : LOW
    );
}


void updateLeds()
{
    bool redOn = overloadActive;
    bool greenOn = (bayStatus == "FREE" && !overloadActive);
    bool yellowOn = (bayStatus == "CHARGING" && !overloadActive);

    digitalWrite(LED_RED, redOn ? HIGH : LOW);
    digitalWrite(LED_GREEN, greenOn ? HIGH : LOW);
    digitalWrite(LED_YELLOW, yellowOn ? HIGH : LOW);

    Serial.print("Bay status: ");
    Serial.print(bayStatus);
    Serial.print(" | Green: ");
    Serial.print(greenOn);
    Serial.print(" | Yellow: ");
    Serial.print(yellowOn);
    Serial.print(" | Red: ");
    Serial.println(redOn);
}
