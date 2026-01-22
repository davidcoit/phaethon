#include "LineAmpPost22113.h"
#include "../../params/ParamMapping.h"
#include <cmath>

void LineAmpPost22113::prepare(double sampleRate)
{
    fs = sampleRate;
    reset();
}

void LineAmpPost22113::reset()
{
}

void LineAmpPost22113::setTrimDb(float newTrimDb)
{
    trimDb = newTrimDb;
}

void LineAmpPost22113::setSaturation(bool enabled)
{
    saturation = enabled;
}

float LineAmpPost22113::processSample(float x)
{
    float y = x * ParamMapping::dbToGain(trimDb);

    if (saturation)
        y = std::tanh(y);

    return y;
}
