#ifndef APP_NETWORK_H
#define APP_NETWORK_H

#include <WiFi.h>
#include <PubSubClient.h>

extern WiFiClient espClient;
extern PubSubClient mqtt;

void connectWiFi();
void connectMQTT();

void mqttCallback(char* topic, byte* payload, unsigned int length);

#endif
