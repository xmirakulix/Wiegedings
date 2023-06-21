#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WifiSecret.h>

void initWifi()
{
    WiFi.mode(WIFI_STA);
    WiFi.hostname(getHostname());
    WiFi.begin(m_Ssid, m_Pass);
    while (!WiFi.isConnected())
    {
        delay(100);
    }
}

void handleWiFi()
{
    if (!WiFi.isConnected())
    {
        WiFi.disconnect();
        initWifi();
    }
}

String getHostname()
{
    return "Wiegedings" + ESP.getChipId();
}