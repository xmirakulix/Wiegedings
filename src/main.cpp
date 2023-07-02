#include <Arduino.h>
#include "Wireless.h"
#include "Mqtt.h"

void setup()
{
    Serial.begin(115200);
    Serial.println("Startup beginning...");

    initWifi();
    initMqtt();

    Serial.println("Startup complete!");
}

void loop()
{
    handleWifi();
    handleMqtt();
}
