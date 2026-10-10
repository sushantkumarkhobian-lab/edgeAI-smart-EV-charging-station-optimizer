#ifndef CONFIG_H
#define CONFIG_H

// ---------------- Pin Configuration ----------------

#define VOLTAGE_PIN 34
#define CURRENT_PIN 35

#define DHT_PIN 15
#define DHT_TYPE DHT22

#define RELAY_PIN 26

#define BTN_PLUGIN 32
#define BTN_PLUGOUT 33

#define LED_GREEN 18
#define LED_YELLOW 19
#define LED_RED 21


// ---------------- WiFi Configuration ----------------

#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASS ""


// ---------------- ThingsBoard Configuration ----------------

#define MQTT_SERVER "mqtt.eu.thingsboard.cloud"
#define MQTT_PORT 1883

#define TB_TOKEN "YOUR_ACCESS_TOKEN"

#define BAY_ID "ESP32_WOKWI"

// ===============================
// Optimization configuration
// ===============================

// Peak tariff period
#define peakTariffStartHr 18
#define peakTariffEndHr 21

// AI arrival probability threshold
#define predictionThreshold 0.50

// Maximum allowed current for one bay
#define overloadCurrentA 8.0

// Maximum station load
#define maxStationLoadW 2000.0

// Relay duty-cycle window
#define DUTY_CYCLE_WINDOW_MS 1000

#endif