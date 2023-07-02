#include "Mqtt.h"
#include "Wireless.h"
#include "Scale.h"
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
        char availTopic[50];
        sprintf(availTopic, "disc/sensor/%s/available", getHostname());

        while (!client.connected())
        {
            Serial.println("MQTT connecting...");
            client.connect(getHostname(), "mosquitto", "mosquitto", availTopic, 1, true, "offline");
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

        generateSensor("Gewicht", deviceCfg, "weight", "g", availTopic);
        generateButton("Tarieren", deviceCfg);
        generateButton("Kalibrieren", deviceCfg);
        generateNumber("Kalibrierung", deviceCfg, "", "");
        generateNumber("Kalibrierungsgewicht", deviceCfg, "weight", "g");
        generateNumber("Flaschengewicht", deviceCfg, "weight", "g");
        generateNumber("Fuellgewicht", deviceCfg, "weight", "g");

        Serial.println("MQTT config messages sent, client state: " + String(client.state()));

        // set sensor to available
        client.publish(availTopic, "online", true);
    }
}

void handleMqtt()
{
    if (!client.loop())
    {
        initMqtt();
    }
}

void generateSensor(const char *name, const char *deviceCfg, const char *deviceClass, const char *unit, const char *availTopic)
{
    char topic[100];
    char stateTopic[100];
    sprintf(topic, "disc/sensor/%s/%s/config", getHostname(), name);
    sprintf(stateTopic, "disc/sensor/%s/%s/state", getHostname(), name);

    char cfgMsg[1024];
    strcpy(cfgMsg, "{");
    sprintf(cfgMsg + strlen(cfgMsg), "  \"name\":          \"%s\",", name);
    sprintf(cfgMsg + strlen(cfgMsg), "  \"unique_id\":     \"%s-%s\",", getHostname(), name);
    sprintf(cfgMsg + strlen(cfgMsg), "  \"device\":        %s ,", deviceCfg);
    sprintf(cfgMsg + strlen(cfgMsg), "  \"device_class\":  \"%s\",", deviceClass);
    sprintf(cfgMsg + strlen(cfgMsg), "  \"unit_of_measurement\":  \"%s\",", unit);
    sprintf(cfgMsg + strlen(cfgMsg), "  \"value_template\": \"{{ value / 1000 }}\",");
    sprintf(cfgMsg + strlen(cfgMsg), "  \"availability_topic\": \"%s\",", availTopic);
    sprintf(cfgMsg + strlen(cfgMsg), "  \"state_topic\":   \"%s\"", stateTopic);
    sprintf(cfgMsg + strlen(cfgMsg), "}");

    Serial.printf("Sending sensor '%s' config message to HA\n", name);
    client.publish(topic, cfgMsg, true);
}

void generateButton(const char *name, const char *deviceCfg)
{
    char topic[100];
    char cmdTopic[100];
    sprintf(topic, "disc/button/%s/%s/config", getHostname(), name);
    sprintf(cmdTopic, "disc/button/%s/%s/command", getHostname(), name);

    char cfgMsg[1024];
    strcpy(cfgMsg, "{");
    sprintf(cfgMsg + strlen(cfgMsg), "  \"name\":         \"%s\",", name);
    sprintf(cfgMsg + strlen(cfgMsg), "  \"unique_id\":    \"%s-%s\",", getHostname(), name);
    sprintf(cfgMsg + strlen(cfgMsg), "  \"device\":        %s ,", deviceCfg);
    sprintf(cfgMsg + strlen(cfgMsg), "  \"payload_press\": \"%s\",", name);
    sprintf(cfgMsg + strlen(cfgMsg), "  \"command_topic\": \"%s\"", cmdTopic);
    sprintf(cfgMsg + strlen(cfgMsg), "}");

    Serial.printf("Sending button '%s' config message to HA\n", name);
    client.publish(topic, cfgMsg, true);

    if (client.subscribe(cmdTopic))
        Serial.println("MQTT subscribed to: " + String(cmdTopic));
}

void generateNumber(const char *name, const char *deviceCfg, const char *deviceClass, const char *unit)
{
    char topic[100];
    char stateTopic[100];
    char cmdTopic[100];
    sprintf(topic, "disc/number/%s/%s/config", getHostname(), name);
    sprintf(stateTopic, "disc/number/%s/%s/state", getHostname(), name);
    sprintf(cmdTopic, "disc/number/%s/%s/command", getHostname(), name);

    char cfgMsg[1024];
    strcpy(cfgMsg, "{");
    sprintf(cfgMsg + strlen(cfgMsg), "  \"name\":         \"%s\",", name);
    sprintf(cfgMsg + strlen(cfgMsg), "  \"unique_id\":    \"%s-%s\",", getHostname(), name);
    sprintf(cfgMsg + strlen(cfgMsg), "  \"device\":        %s ,", deviceCfg);
    if (strcmp(deviceClass, "") != 0)
        sprintf(cfgMsg + strlen(cfgMsg), "  \"device_class\": \"%s\",", deviceClass);
    sprintf(cfgMsg + strlen(cfgMsg), "  \"unit_of_measurement\":  \"%s\",", unit);
    sprintf(cfgMsg + strlen(cfgMsg), "  \"mode\":             \"box\",");
    sprintf(cfgMsg + strlen(cfgMsg), "  \"min\":             \"0\",");
    sprintf(cfgMsg + strlen(cfgMsg), "  \"max\":             \"100000\",");
    sprintf(cfgMsg + strlen(cfgMsg), "  \"step\":             \"0.001\",");
    sprintf(cfgMsg + strlen(cfgMsg), "  \"entity_category\":  \"config\",");
    sprintf(cfgMsg + strlen(cfgMsg), "  \"command_topic\":    \"%s\",", cmdTopic);
    sprintf(cfgMsg + strlen(cfgMsg), "  \"command_template\": \"{{ value * 1000 }}\",");
    sprintf(cfgMsg + strlen(cfgMsg), "  \"state_topic\":      \"%s\",", stateTopic);
    sprintf(cfgMsg + strlen(cfgMsg), "  \"state_template\": \"{{ value / 1000 }}\"");
    sprintf(cfgMsg + strlen(cfgMsg), "}");

    Serial.printf("Sending number '%s' config message to HA\n", name);
    client.publish(topic, cfgMsg, true);

    if (client.subscribe(cmdTopic))
        Serial.println("MQTT subscribed to: " + String(cmdTopic));
}

void onMqtt(char *topic, byte *payload, unsigned int length)
{
    char *topicParts[5];
    char *ptr = NULL;
    byte i = 0;

    char origTopic[100];
    strcpy(origTopic, topic);

    ptr = strtok(topic, "/");
    while (ptr != NULL)
    {
        if (i == 5)
        {
            Serial.println("MQTT received invalid topic: " + String(origTopic));
            return;
        }
        topicParts[i] = ptr;
        i++;
        ptr = strtok(NULL, "/");
    }

    if (i != 5)
    {
        Serial.println("MQTT received invalid topic: " + String(origTopic));
        return;
    }

    char msg[200] = "";
    for (byte i = 0; i < length; i++)
    {
        msg[i] = (char)payload[i];
    }

    Serial.println("MQTT received topic: " + String(origTopic) + " with payload: " + String(msg));

    /**
     * state:
     *  Kalibrierungsgewicht
     *  Flaschengewicht
     *  Fuellgewicht
     *
     * cmd:
     *  Tarieren
     *  Kalibrieren
     */

    if (strcmp(topicParts[3], "Kalibrierungsgewicht") == 0)
        setCalibreWeight(atof(msg));
    else if (strcmp(topicParts[3], "Flaschengewicht") == 0)
        setBottleWeight(atoi(msg));
    else if (strcmp(topicParts[3], "Fuellgewicht") == 0)
        setContentFullWeight(atoi(msg));
    else if (strcmp(topicParts[3], "Tarieren") == 0)
        tare();
    else if (strcmp(topicParts[3], "Kalibrieren") == 0)
        calibrate();
    else
    {
        Serial.println("MQTT could not parse topic, aborting...");
        return;
    }
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
*/