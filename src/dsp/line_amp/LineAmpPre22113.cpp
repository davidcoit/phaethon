#include "LineAmpPre22113.h"
#include "../../params/ParamMapping.h"

void LineAmpPre22113::prepare(double sampleRate)
{
    fs = sampleRate;
    reset();
}

void LineAmpPre22113::reset()
{
}

void LineAmpPre22113::setDriveDb(float newDriveDb)
{
    driveDb = newDriveDb;
}

float LineAmpPre22113::processSample(float x)
{
    const float g = ParamMapping::dbToGain(driveDb);
    return x * g;
}
