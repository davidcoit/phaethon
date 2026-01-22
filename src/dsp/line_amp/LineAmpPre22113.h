#pragma once

#include <juce_dsp/juce_dsp.h>

class LineAmpPre22113
{
public:
    void prepare(double sampleRate);
    void reset();
    void setDriveDb(float driveDb);
    float processSample(float x);

private:
    double fs { 44100.0 };
    float driveDb { 0.0f };
};
