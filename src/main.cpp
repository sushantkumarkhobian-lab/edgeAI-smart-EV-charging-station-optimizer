#include <Arduino.h>
#include <time.h>

#include "config.h"
#include "AppNetwork.h"
#include "Peripherals.h"
#include "EdgeAi.h"
#include "optimization.h"
#include "telemetry.h"

void setup()
{
    Serial.begin(115200);

    dht.begin();

    // Relay
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW);

    // Buttons
    pinMode(BTN_PLUGIN, INPUT_PULLUP);
    pinMode(BTN_PLUGOUT, INPUT_PULLUP);

    // LEDs
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_YELLOW, OUTPUT);
    pinMode(LED_RED, OUTPUT);

    mqtt.setServer(MQTT_SERVER, MQTT_PORT);

    connectWiFi();

    configTime(
        19800,
        0,
        "pool.ntp.org",
        "time.nist.gov"
    );

    connectMQTT();

    Serial.println("System initialized.");
}

void loop()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        connectWiFi();
    }

    if (!mqtt.connected())
    {
        connectMQTT();
    }

    mqtt.loop();

    // 1. Read sensors
    printval();

    // 2. Check plug-in / plug-out
    plug_status();

    // 3. Run Edge AI
    runEdgeAIInference();

    // 4. Run optimization
    runOptimization();

    // 5. Apply optimized relay control
    applyRelayDutyCycle();

    // 6. Update status LEDs
    updateLeds();

    // 7. Send telemetry
    publishTelemetry();

    delay(5000);
}