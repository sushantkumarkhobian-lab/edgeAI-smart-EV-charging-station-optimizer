#include <Arduino.h>
#include <DHT.h>

#include "State.h"
#include "Peripherals.h"
#include "MapFloat.h"
#include "config.h"

DHT dht(DHT_PIN, DHT_TYPE);

void printval(void)
{
    long currentRaw = analogRead(CURRENT_PIN);
    long voltageRaw = analogRead(VOLTAGE_PIN);

    current = mapFloat(
        currentRaw,
        0,
        4095,
        0.0,
        10.0
    );

    voltage = mapFloat(
        voltageRaw,
        0,
        4095,
        0.0,
        230.0
    );

    temperature = dht.readTemperature();

    power = voltage * current;

    Serial.print("Current: ");
    Serial.print(current, 2);
    Serial.println(" A");

    Serial.print("Voltage: ");
    Serial.print(voltage, 2);
    Serial.println(" V");

    Serial.print("Power: ");
    Serial.print(power, 2);
    Serial.println(" W");

    if (isnan(temperature))
    {
        Serial.println("ERROR in reading the Temperature");
    }
    else
    {
        Serial.print("Temperature: ");
        Serial.print(temperature, 1);
        Serial.println(" C");
    }
}


void plug_status(void)
{
    static bool previousPluginState = HIGH;
    static bool previousPlugoutState = HIGH;

    bool pluginState = digitalRead(BTN_PLUGIN);
    bool plugoutState = digitalRead(BTN_PLUGOUT);

    if (pluginState == LOW)
    {
        Serial.println("Green button is PRESSED");
    }

    if (plugoutState == LOW)
    {
        Serial.println("Red button is PRESSED");
    }

    if (previousPluginState == HIGH && pluginState == LOW)
    {
        bayStatus = "CHARGING";
        Serial.println("Bay status changed to CHARGING");
    }

    if (previousPlugoutState == HIGH && plugoutState == LOW)
    {
        bayStatus = "FREE";
        overloadActive = false;
        throttleLevel = 100;
        Serial.println("Bay status changed to FREE");
    }

    previousPluginState = pluginState;
    previousPlugoutState = plugoutState;
}


float recentAvgCurrent()
{
    static float samples[10] = {0};
    static int index = 0;
    static int count = 0;

    samples[index] = current;

    index = (index + 1) % 10;

    if (count < 10)
        count++;

    float sum = 0.0;

    for (int i = 0; i < count; i++)
    {
        sum += samples[i];
    }

    return sum / count;
}