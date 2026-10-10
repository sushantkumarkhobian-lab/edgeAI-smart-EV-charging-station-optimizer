
#include <Arduino.h>
#include <ArduinoJson.h>

#include "rpc.h"
#include "AppNetwork.h"
#include "State.h"
#include "config.h"

// RPC command handling
void handleRpc(String requestId, char* payload)
{
    StaticJsonDocument<200> doc;

    if (deserializeJson(doc, payload))
        return;

    String method = doc["method"] | "";
    JsonObject params = doc["params"].as<JsonObject>();

    StaticJsonDocument<200> response;

    if (method == "setRelayState")
    {
        bool state = params["state"] | false;

        manualOverrideActive = true;
        throttleLevel = state ? 100 : 0;
        loadDecision = state ? "ALLOW" : "MANUAL_OFF";

        response["success"] = true;
        response["throttleLevel"] = throttleLevel;

        Serial.print(">> RPC setRelayState(");
        Serial.print(state);
        Serial.println(") - manual override engaged.");
    }
    else if (method == "setThrottle")
    {
        int level = params["level"] | 100;
        level = constrain(level, 0, 100);

        manualOverrideActive = true;
        throttleLevel = level;
        loadDecision = "MANUAL_THROTTLE";

        response["success"] = true;
        response["throttleLevel"] = throttleLevel;

        Serial.print(">> RPC setThrottle(");
        Serial.print(level);
        Serial.println(") - manual override engaged.");
    }
    else
    {
        response["success"] = false;
        response["error"] = "unknown method";
    }

    char buffer[220];
    serializeJson(response, buffer, sizeof(buffer));

    String responseTopic =
        "v1/devices/me/rpc/response/" + requestId;

    mqtt.publish(responseTopic.c_str(), buffer);
}
