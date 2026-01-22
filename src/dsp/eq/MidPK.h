#pragma once

#include <juce_dsp/juce_dsp.h>
#include "../wdf/WdfElements.h"

class MidPK
{
public:
    enum class FreqPos
    {
        Pos0_7 = 0,
        Pos1_0,
        Pos1_4,
        Pos2_0,
        Pos2_8,
        Pos3_5,
        Pos4_5,
        Pos6_0
    };

    void prepare(double sampleRate, int fadeMs);
    void reset();
    void setFreq(FreqPos newFreq);
    void setGainDb(float gainDb);
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

    void beginTransition(FreqPos newFreq);

    double fs { 44100.0 };
    bool transitioning { false };
    FreqPos currentFreq { FreqPos::Pos1_0 };
    FreqPos pendingFreq { FreqPos::Pos1_0 };

    WdfNetwork current;
    WdfNetwork next;

    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> xfade;

    float gainDb { 0.0f };
};
