#pragma once

#include <juce_dsp/juce_dsp.h>

class EqCut
{
public:
    void prepare(double sampleRate, int fadeMs);
    void reset();
    void setBypass(bool bypassed);
    float processSample(float x);

private:
    double fs { 44100.0 };
    bool bypass { false };
    bool transitioning { false };

    juce::dsp::IIR::Filter<double> shuntFilter;
    float padGain { 1.0f };

    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> xfade;
};
