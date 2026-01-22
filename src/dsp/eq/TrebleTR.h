#pragma once

#include <juce_dsp/juce_dsp.h>

class TrebleTR
{
public:
    enum class FreqPos
    {
        Pos2 = 0,
        Pos4,
        Pos6,
        Pos8,
        Pos10
    };

    void prepare(double sampleRate, int fadeMs);
    void reset();
    void setFreq(FreqPos newFreq);
    void setGainDb(float gainDb);
    float processSample(float x);

private:
    struct ShelfFilter
    {
        juce::dsp::IIR::Filter<double> filter;
        void reset();
        void setCoefficients(double sampleRate, double freqHz, double gainDb);
        double process(double x);
    };

    void beginFreqTransition(FreqPos newFreq);

    double fs { 44100.0 };
    bool transitioning { false };
    FreqPos currentFreq { FreqPos::Pos2 };
    FreqPos pendingFreq { FreqPos::Pos2 };

    ShelfFilter cutCurrent;
    ShelfFilter cutNext;
    ShelfFilter boostCurrent;
    ShelfFilter boostNext;

    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> freqXfade;

    float gainDb { 0.0f };
    const float maxAbsGainDb { 12.0f };
};
