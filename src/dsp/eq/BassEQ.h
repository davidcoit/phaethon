#pragma once

#include <juce_dsp/juce_dsp.h>
#include "../wdf/WdfElements.h"

class BassEQ
{
public:
    enum class FreqPos
    {
        Pos0 = 0,
        Pos60,
        Pos120,
        Pos250,
        Pos400
    };

    void prepare(double sampleRate, int fadeMs);
    void reset();
    void setFreq(FreqPos newFreq);
    void setGainDb(float gainDb);
    void set50HzCut(bool enabled);
    float processSample(float x);

private:
    struct WdfNetwork
    {
        void prepare(double sampleRate);
        void reset();
        void setLC(double inductanceH, double capacitanceF);
        void setSeriesR(double rOhms);
        double process(double x);

        wdf::Resistor seriesR;
        wdf::Capacitor cap;
        wdf::Inductor ind;
        wdf::ParallelAdaptor lcParallel { ind, cap };
        wdf::SeriesAdaptor network { seriesR, lcParallel };

        double fs { 44100.0 };
    };

    struct HPFilter
    {
        juce::dsp::IIR::Filter<double> filter;
        void reset();
        void setCoefficients(double sampleRate, double freqHz);
        double process(double x);
    };

    void beginTransition(FreqPos newFreq);

    double fs { 44100.0 };
    bool transitioning { false };
    FreqPos currentFreq { FreqPos::Pos0 };
    FreqPos pendingFreq { FreqPos::Pos0 };

    WdfNetwork boostCurrent;
    WdfNetwork boostNext;
    WdfNetwork cutCurrent;
    WdfNetwork cutNext;

    HPFilter cut50Current;
    HPFilter cut50Next;
    bool cut50Enabled { false };
    bool cut50Transitioning { false };

    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> freqXfade;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> cut50Xfade;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> gainXfade;

    float gainDb { 0.0f };
};
