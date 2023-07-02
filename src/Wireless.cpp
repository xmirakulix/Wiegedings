#include "Wireless.h"
#include "WifiSecret.h"

WiFiClient wifiClient;
char hostname[20] = "";

void initWifi()
{
    sprintf(hostname, "Wiegedings-%X", ESP.getChipId());

    Serial.print("WiFi init, Hostname: ");
    Serial.println(getHostname());
    WiFi.mode(WIFI_STA);
    WiFi.hostname(getHostname());
    WiFi.begin(m_Ssid, m_Pass);
    Serial.print("WiFi connecting");

    while (!WiFi.isConnected())
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println();

    Serial.print("WiFi connected, IP address: ");
    Serial.println(WiFi.localIP());
}

void handleWifi()
{
    if (!WiFi.isConnected())
    {
        Serial.println("WiFi not connected, reconnecting...");
        WiFi.disconnect();
        initWifi();
    }
}

char *getHostname()
{
    return hostname;
}