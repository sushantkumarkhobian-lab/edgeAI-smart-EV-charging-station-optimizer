
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <cstring>

#include "AppNetwork.h"
#include "config.h"
#include "attributes.h"
#include "rpc.h"

// ---------------------------------------------------------------------
// WiFi and MQTT objects
// ---------------------------------------------------------------------

WiFiClient espClient;
PubSubClient mqtt(espClient);

// ---------------------------------------------------------------------
// Connect to WiFi
// ---------------------------------------------------------------------

void connectWiFi()
{
    Serial.print("Connecting to WiFi");

    WiFi.begin(WIFI_SSID, WIFI_PASS);

    unsigned long start = millis();

    while (WiFi.status() != WL_CONNECTED &&
           millis() - start < 15000)
    {
        delay(300);
        Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println(" connected.");
        Serial.print("IP: ");
        Serial.println(WiFi.localIP());
    }
    else
    {
        Serial.println(" FAILED (will retry).");
    }
}

// ---------------------------------------------------------------------
// Connect to ThingsBoard MQTT
// ---------------------------------------------------------------------

void connectMQTT()
{
    if (WiFi.status() != WL_CONNECTED)
        return;

    // Configure the callback before connecting.
    mqtt.setCallback(mqttCallback);

    Serial.print("Connecting to ThingsBoard MQTT...");

    if (mqtt.connect(BAY_ID, TB_TOKEN, NULL))
    {
        Serial.println(" connected.");

        // Subscribe to shared attribute updates.
        mqtt.subscribe("v1/devices/me/attributes");

        // Subscribe to shared attribute request responses.
        mqtt.subscribe("v1/devices/me/attributes/response/+");

        // Subscribe to server-side RPC requests.
        mqtt.subscribe("v1/devices/me/rpc/request/+");

        // Request the current shared attributes.
        requestSharedAttributes();
    }
    else
    {
        Serial.print(" failed, rc=");
        Serial.println(mqtt.state());

        delay(1000);
    }
}

// ---------------------------------------------------------------------
// Handle incoming MQTT messages
// ---------------------------------------------------------------------

void mqttCallback(char* topic, byte* payload, unsigned int length)
{
    String topicStr = String(topic);

    // Copy the MQTT payload into a null-terminated buffer.
    char buf[400];

    unsigned int n = length < sizeof(buf) - 1
                         ? length
                         : sizeof(buf) - 1;

    memcpy(buf, payload, n);
    buf[n] = '\0';

    Serial.print("[MQTT <<] ");
    Serial.print(topicStr);
    Serial.print(" ");
    Serial.println(buf);

    // -------------------------------------------------------------
    // Handle RPC requests
    // -------------------------------------------------------------

    if (topicStr.startsWith("v1/devices/me/rpc/request/"))
    {
        String requestId =
            topicStr.substring(topicStr.lastIndexOf('/') + 1);

        handleRpc(requestId, buf);
        return;
    }

    // -------------------------------------------------------------
    // Handle shared attribute updates and responses
    // -------------------------------------------------------------

    StaticJsonDocument<256> doc;

    DeserializationError err = deserializeJson(doc, buf);

    if (err)
    {
        Serial.print("JSON parsing failed: ");
        Serial.println(err.c_str());
        return;
    }

    JsonObject attrs;

    if (doc["shared"].is<JsonObject>())
    {
        attrs = doc["shared"].as<JsonObject>();
    }
    else
    {
        attrs = doc.as<JsonObject>();
    }

    applySharedAttributes(attrs);
}
