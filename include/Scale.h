#include <Arduino.h>

void setWeight(unsigned int w);
unsigned int getWeight();

void setBottleWeight(unsigned int w);
unsigned int getBottleWeight();

void setContentFullWeight(unsigned int w);
unsigned int getContentFullWeight();

void setCalibreWeight(unsigned int w);
unsigned int getCalibreWeight();

void setCalibrationFactor(unsigned int f);
unsigned int getCalibrationFactor();

void tare();
void calibrate();