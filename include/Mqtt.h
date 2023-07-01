#include <Arduino.h>

void mqtt_setup();
void mqtt_callback(char *topic, byte *payload, unsigned int length);
