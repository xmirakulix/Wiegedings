#include "Mqtt.h"
#include "Wireless.h"
#include <PubSubClient.h>

PubSubClient client(wifiClient);
const char m_CompileDate[] = __DATE__ " " __TIME__;

void mqtt_setup()
{
    client.setServer("homeassistant", 1883);
    client.setCallback(mqtt_callback);
    client.setBufferSize(1024);

    // connect to MQTT broker
    if (!client.connected())
    {
        while (!client.connected())
        {
            Serial.println("Connecting to MQTT...");
            client.connect("Regnerdings", "mosquitto", "mosquitto", "/home/state", 1, true, "offline");
            delay(100);
        }
        Serial.println("Connected to MQTT");
        delay(2000);

        char topic[100];
        char cfgMsg[1024];

        // configure device temp
        strcpy(topic, "disc/sensor/Regnerdings/T/config");
        strcpy(cfgMsg, "{");
        sprintf(cfgMsg + strlen(cfgMsg), "  \"name\":          \"Temperature\",");
        sprintf(cfgMsg + strlen(cfgMsg), "  \"unique_id\":     \"Temperature\",");
        sprintf(cfgMsg + strlen(cfgMsg), "  \"device\":        { \"ids\" : [\"Regnerdings\"], \"name\" : \"Regnerdings\", \"mf\": \"xmirakulix\", \"mdl\": \"Regnerdings\", \"sw\": \"%s\" },", m_CompileDate);
        sprintf(cfgMsg + strlen(cfgMsg), "  \"device_class\":  \"temperature\",");
        sprintf(cfgMsg + strlen(cfgMsg), "  \"unit_of_measurement\":  \"°C\",");
        sprintf(cfgMsg + strlen(cfgMsg), "  \"value_template\":\"{{ value_json.temperature}}\",");
        sprintf(cfgMsg + strlen(cfgMsg), "  \"state_topic\":   \"disc/sensor/Regnerdings/state\"");
        sprintf(cfgMsg + strlen(cfgMsg), "}");

        Serial.println("Sending temperature config message to HA");
        client.publish(topic, cfgMsg, true);

        // configure device humidity
        strcpy(topic, "disc/sensor/Regnerdings/H/config");
        strcpy(cfgMsg, "{");
        sprintf(cfgMsg + strlen(cfgMsg), "  \"name\":          \"Humidity\",");
        sprintf(cfgMsg + strlen(cfgMsg), "  \"unique_id\":     \"Humidity\",");
        sprintf(cfgMsg + strlen(cfgMsg), "  \"device\":        { \"identifiers\" : [\"Regnerdings\"], \"name\" : \"Regnerdings\" },");
        sprintf(cfgMsg + strlen(cfgMsg), "  \"device_class\":  \"humidity\",");
        sprintf(cfgMsg + strlen(cfgMsg), "  \"unit_of_measurement\":  \"%%\",");
        sprintf(cfgMsg + strlen(cfgMsg), "  \"value_template\":\"{{ value_json.humidity}}\",");
        sprintf(cfgMsg + strlen(cfgMsg), "  \"state_topic\":   \"disc/sensor/Regnerdings/state\"");
        sprintf(cfgMsg + strlen(cfgMsg), "}");

        Serial.println("Sending humidity config message to HA");
        client.publish(topic, cfgMsg, true);
        /*
                for (int i = 1; i <= NUMPORTS; i++)
                {
                    char cmd_topic[50];

                    // configure Ports
                    sprintf(topic, "disc/switch/Regnerdings/P%d/config", i);
                    sprintf(cmd_topic, "disc/switch/Regnerdings/P%d/set", i);

                    strcpy(cfgMsg, "{");
                    sprintf(cfgMsg + strlen(cfgMsg), "  \"name\":          \"Regnerdings P%d\",", i);
                    sprintf(cfgMsg + strlen(cfgMsg), "  \"unique_id\":     \"Regnerdings_P%d\",", i);
                    sprintf(cfgMsg + strlen(cfgMsg), "  \"device\":        { \"identifiers\" : [\"Regnerdings\"], \"name\" : \"Regnerdings\" },");
                    sprintf(cfgMsg + strlen(cfgMsg), "  \"device_class\":  \"switch\",");
                    sprintf(cfgMsg + strlen(cfgMsg), "  \"icon\":          \"mdi:sprinkler\",");
                    sprintf(cfgMsg + strlen(cfgMsg), "  \"value_template\":\"{{ value_json.P%d}}\",", i);
                    sprintf(cfgMsg + strlen(cfgMsg), "  \"state_topic\":   \"disc/switch/Regnerdings/state\",");
                    sprintf(cfgMsg + strlen(cfgMsg), "  \"command_topic\": \"%s\"", cmd_topic);
                    sprintf(cfgMsg + strlen(cfgMsg), "}");

                    Serial.printf("Sending P%d config message to HA\n", i);
                    client.publish(topic, cfgMsg, true);
                    client.subscribe(cmd_topic);

                    pinMode(m_Ports[i - 1], OUTPUT);
                    setSwitch(i, false);
                }
        */
        Serial.println("Config message sent, client state: " + String(client.state()));
        delay(2000);
    }
}

void mqtt_callback(char *topic, byte *payload, unsigned int length)
{
    char msg[50] = "";
    for (byte i = 0; i < length; i++)
    {
        msg[i] = (char)payload[i];
    }

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