#pragma once

#include <juce_dsp/juce_dsp.h>
#include "eq/FilterHPF.h"
#include "eq/TrebleTR.h"
#include "eq/MidPK.h"
#include "eq/BassEQ.h"
#include "eq/EqCut.h"
#include "line_amp/LineAmpPre22113.h"
#include "line_amp/LineAmpPost22113.h"

class Helios69Chain
{
public:
    void prepare(double sampleRate, int samplesPerBlock);
    void reset();

    void setInputTrimDb(float trimDb);
    void setFilterMode(int modeIndex);
    void setTrGainDb(float gainDb);
    void setMidFreq(int index);
    void setMidGainDb(float gainDb);
    void setBassFreq(int index);
    void setBassGainDb(float gainDb);
    void setBass50Hz(bool enabled);
    void setEqCut(bool enabled);
    void setPostTrimDb(float trimDb);
    void setSaturation(bool enabled);

    float processSample(float x);

private:
    double fs { 44100.0 };
    float inputGain { 1.0f };

    FilterHPF filter;
    LineAmpPre22113 preLineAmp;
    TrebleTR treble;
    MidPK mid;
    BassEQ bass;
    EqCut eqCut;
    LineAmpPost22113 postLineAmp;

};
