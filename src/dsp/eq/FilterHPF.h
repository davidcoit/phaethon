#pragma once

#include <juce_dsp/juce_dsp.h>

class FilterHPF
{
public:
    enum class Mode
    {
        Off = 0,
        Hz80 = 1,
        Hz140 = 2
    };

    void prepare(double sampleRate, int fadeMs);
    void reset();
    void setMode(Mode newMode);
    float processSample(float x);

private:
    struct TwoPoleHPF
    {
        juce::dsp::IIR::Filter<double> f1;
        juce::dsp::IIR::Filter<double> f2;

        void reset();
        void setCutoff(double sampleRate, double hz);
        double process(double x);
    };

    void beginTransition(Mode newMode);

    double fs { 44100.0 };
    int fadeSamples { 0 };

    Mode currentMode { Mode::Off };
    Mode pendingMode { Mode::Off };
    bool transitioning { false };

    TwoPoleHPF current;
    TwoPoleHPF next;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> xfade;
};
