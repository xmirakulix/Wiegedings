#include "Mqtt.h"
#include "Wireless.h"
#include <PubSubClient.h>

PubSubClient client(wifiClient);

void initMqtt()
{
    Serial.println("MQTT init started...");
    client.setServer("homeassistant", 1883);
    client.setCallback(onMqtt);
    client.setBufferSize(1024);

    // connect to MQTT broker
    if (!client.connected())
    {
        while (!client.connected())
        {
            Serial.println("MQTT connecting...");
            client.connect(getHostname(), "mosquitto", "mosquitto", "/home/state", 1, true, "offline");
            delay(100);
        }
        Serial.println("MQTT connected!");
        delay(500);

        /**
         * Sensor | Type "weight"
         * Button | Tarieren
         * Button | Kalibrierung durchführen
         * Number | Ergebnis der Kalibrierung
         * Number | Gewicht zum Kalibrieren
         * Number | Flasche Eigengewicht
         * Number | Flasche Gewicht Füllung
         */

        char compileDate[] = __DATE__ " " __TIME__;
        char deviceCfg[500];
        sprintf(deviceCfg, "{ \"ids\" : [\"%s\"], \"name\" : \"Wiegedings\", \"mf\": \"xmirakulix\", \"sw\": \"%s\" } ", getHostname(), compileDate);

        generateSensor("Gewicht", deviceCfg, "weight", "g");
        generateButton("Tarieren", deviceCfg);
        generateButton("Kalibrieren", deviceCfg);
        generateNumber("Kalibrierung", deviceCfg, "", "");
        generateNumber("Kalibrierungsgewicht", deviceCfg, "weight", "g");
        generateNumber("Flaschengewicht", deviceCfg, "weight", "g");
        generateNumber("Fuellgewicht", deviceCfg, "weight", "g");

        Serial.println("MQTT config messages sent, client state: " + String(client.state()));
    }
}

void generateSensor(const char *name, const char *deviceCfg, const char *deviceClass, const char *unit)
{
    char topic[100];
    char cfgMsg[1024];

    sprintf(topic, "disc/sensor/%s/%s/config", getHostname(), name);
    strcpy(cfgMsg, "{");
    sprintf(cfgMsg + strlen(cfgMsg), "  \"name\":          \"%s\",", name);
    sprintf(cfgMsg + strlen(cfgMsg), "  \"unique_id\":     \"%s-%s\",", getHostname(), name);
    sprintf(cfgMsg + strlen(cfgMsg), "  \"device\":        %s ,", deviceCfg);
    sprintf(cfgMsg + strlen(cfgMsg), "  \"device_class\":  \"%s\",", deviceClass);
    sprintf(cfgMsg + strlen(cfgMsg), "  \"unit_of_measurement\":  \"%s\",", unit);
    sprintf(cfgMsg + strlen(cfgMsg), "  \"value_template\":\"{{ value_json.%s }}\",", name);
    sprintf(cfgMsg + strlen(cfgMsg), "  \"state_topic\":   \"disc/sensor/%s/state\"", getHostname());
    sprintf(cfgMsg + strlen(cfgMsg), "}");

    Serial.printf("Sending sensor '%s' config message to HA\n", name);
    client.publish(topic, cfgMsg, true);
}

void generateButton(const char *name, const char *deviceCfg)
{
    char topic[100];
    char cfgMsg[1024];

    sprintf(topic, "disc/button/%s/%s/config", getHostname(), name);
    strcpy(cfgMsg, "{");
    sprintf(cfgMsg + strlen(cfgMsg), "  \"name\":         \"%s\",", name);
    sprintf(cfgMsg + strlen(cfgMsg), "  \"unique_id\":    \"%s-%s\",", getHostname(), name);
    sprintf(cfgMsg + strlen(cfgMsg), "  \"device\":        %s ,", deviceCfg);
    sprintf(cfgMsg + strlen(cfgMsg), "  \"payload_press\": \"%s\",", name);
    sprintf(cfgMsg + strlen(cfgMsg), "  \"command_topic\": \"disc/button/%s/command\"", getHostname());
    sprintf(cfgMsg + strlen(cfgMsg), "}");

    Serial.printf("Sending button '%s' config message to HA\n", name);
    client.publish(topic, cfgMsg, true);
}

void generateNumber(const char *name, const char *deviceCfg, const char *deviceClass, const char *unit)
{
    char topic[100];
    char cfgMsg[1024];

    sprintf(topic, "disc/number/%s/%s/config", getHostname(), name);
    strcpy(cfgMsg, "{");
    sprintf(cfgMsg + strlen(cfgMsg), "  \"name\":         \"%s\",", name);
    sprintf(cfgMsg + strlen(cfgMsg), "  \"unique_id\":    \"%s-%s\",", getHostname(), name);
    sprintf(cfgMsg + strlen(cfgMsg), "  \"device\":        %s ,", deviceCfg);
    if (strcmp(deviceClass, "") != 0)
        sprintf(cfgMsg + strlen(cfgMsg), "  \"device_class\": \"%s\",", deviceClass);
    sprintf(cfgMsg + strlen(cfgMsg), "  \"unit_of_measurement\":  \"%s\",", unit);
    sprintf(cfgMsg + strlen(cfgMsg), "  \"mode\":             \"box\",");
    sprintf(cfgMsg + strlen(cfgMsg), "  \"entity_category\":  \"config\",");
    sprintf(cfgMsg + strlen(cfgMsg), "  \"command_template\": \"{ \\\"%s\\\": {{ value }} }\",", name);
    sprintf(cfgMsg + strlen(cfgMsg), "  \"command_topic\":    \"disc/number/%s/command\",", getHostname());
    sprintf(cfgMsg + strlen(cfgMsg), "  \"value_template\":   \"{{ value_json.%s }}\",", name);
    sprintf(cfgMsg + strlen(cfgMsg), "  \"state_topic\":      \"disc/number/%s/state\"", getHostname());
    sprintf(cfgMsg + strlen(cfgMsg), "}");

    Serial.printf("Sending number '%s' config message to HA\n", name);
    client.publish(topic, cfgMsg, true);
}

void onMqtt(char *topic, byte *payload, unsigned int length)
{
    char msg[50] = "";
    for (byte i = 0; i < length; i++)
    {
        msg[i] = (char)payload[i];
    }
    Serial.println("MQTT received: " + String(msg));

    /*     int port = parsePort(topic);
        Serial.printf("Message received on port '%d': %s\n", port, msg);

        if (strcmp(msg, "ON") == 0)
        {
            setSwitch(port, true);
        }
        else if (strcmp(msg, "OFF") == 0)
        {
            setSwitch(port, false);
        }
        else
        {
            Serial.printf("Unknown message: %s\n", msg);
        }*/
}
/*
// transmit temperature and humidity to HA
void txTemperatureState()
{
    m_LastTemperature = read_temperature();
    m_LastHumidity = read_humidity();

    char mqtt_state[100];
    sprintf(mqtt_state, "{ \"temperature\": %d, \"humidity\": %d }", m_LastTemperature, m_LastHumidity);

    Serial.printf("Temp/Humid state: %s\n", mqtt_state);
    client.publish("disc/sensor/Regnerdings/state", mqtt_state, false);
}

// transmit state of all switches to HA
void txSwitchState()
{
    String mqtt_state = "{";

    for (int i = 1; i <= NUMPORTS; i++)
    {
        mqtt_state += "\"P" + String(i) + "\" : \"" + getSwitch(i) + "\", ";
    }
    mqtt_state = mqtt_state.substring(0, mqtt_state.length() - 2);
    mqtt_state += "}";

    Serial.printf("Switch state: %s\n", mqtt_state.c_str());
    client.publish("disc/switch/Regnerdings/state", mqtt_state.c_str(), false);

    setActivityLed();
}
*/