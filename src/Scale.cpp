#include "Scale.h"

unsigned int weight = 0;
unsigned int bottleWeight = 0;
unsigned int calibreWeight = 0;
unsigned int contentFullWeight = 0;

unsigned int calibrationFactor = 0;

void tare()
{
    Serial.println("Scale starting taring...");
}

void calibrate()
{
    Serial.println("Scale starting calibration...");
}

void setWeight(unsigned int w)
{
    weight = w;
}
unsigned int getWeight()
{
    return weight;
}

void setBottleWeight(unsigned int w)
{
    bottleWeight = w;
}

unsigned int getBottleWeight()
{
    return bottleWeight;
}

void setContentFullWeight(unsigned int w)
{
    contentFullWeight = w;
}

unsigned int getContentFullWeight()
{
    return contentFullWeight;
}

void setCalibreWeight(unsigned int w)
{
    calibreWeight = w;
}

unsigned int getCalibreWeight()
{
    return calibreWeight;
}

void setCalibrationFactor(unsigned int f)
{
    calibrationFactor = f;
}

unsigned int getCalibrationFactor()
{
    return calibrationFactor;
}
