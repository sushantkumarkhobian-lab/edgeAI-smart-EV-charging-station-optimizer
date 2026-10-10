
#include <Arduino.h>
#include <ArduinoJson.h>
#include "config.h"

#include "attributes.h"
#include "AppNetwork.h"
#include "State.h"

// Request the current shared attributes from ThingsBoard.
void requestSharedAttributes()
{
    StaticJsonDocument<128> doc;

    doc["sharedKeys"] =
        "maxStationLoadW,overloadCurrentA,peakTariffStartHr,"
        "peakTariffEndHr,predictionThreshold";

    char buffer[128];
    serializeJson(doc, buffer, sizeof(buffer));

    mqtt.publish(
        "v1/devices/me/attributes/request/1",
        buffer
    );
}

// Apply shared attributes received from ThingsBoard.

void applySharedAttributes(JsonObject attrs) {
    Serial.println("Shared attributes received.");

    Serial.print("Max station load (W): ");
    Serial.println(maxStationLoadW);

    Serial.print("Overload current (A): ");
    Serial.println(overloadCurrentA);

    Serial.print("Peak tariff start hour: ");
    Serial.println(peakTariffStartHr);

    Serial.print("Peak tariff end hour: ");
    Serial.println(peakTariffEndHr);

    Serial.print("Prediction threshold: ");
    Serial.println(predictionThreshold);
}

