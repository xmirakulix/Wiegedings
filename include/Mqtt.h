#include <Arduino.h>

void initMqtt();
void handleMqtt();

void generateSensor(const char *name, const char *deviceCfg, const char *deviceClass, const char *unit, const char *availTopic);
void generateButton(const char *name, const char *deviceCfg);
void generateNumber(const char *name, const char *deviceCfg, const char *deviceClass, const char *unit);

void onMqtt(char *topic, byte *payload, unsigned int length);
