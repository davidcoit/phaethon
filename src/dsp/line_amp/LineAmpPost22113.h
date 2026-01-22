#pragma once

#include <juce_dsp/juce_dsp.h>

class LineAmpPost22113
{
public:
    void prepare(double sampleRate);
    void reset();
    void setTrimDb(float trimDb);
    void setSaturation(bool enabled);
    float processSample(float x);

private:
    double fs { 44100.0 };
    float trimDb { 0.0f };
    bool saturation { false };
};
