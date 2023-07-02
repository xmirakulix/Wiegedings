#include <Arduino.h>
#include <ESP8266WiFi.h>

extern WiFiClient wifiClient;

void initWifi();
void handleWifi();

char *getHostname();
