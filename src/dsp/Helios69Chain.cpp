#include "Helios69Chain.h"
#include "../params/ParamMapping.h"

void Helios69Chain::prepare(double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused(samplesPerBlock);
    fs = sampleRate;
    filter.prepare(sampleRate, 20);
    treble.prepare(sampleRate, 20);
    treble.setFreq(TrebleTR::FreqPos::Pos10);
    mid.prepare(sampleRate, 20);
    bass.prepare(sampleRate, 20);
    eqCut.prepare(sampleRate, 20);
    preLineAmp.prepare(sampleRate);
    postLineAmp.prepare(sampleRate);

    reset();
}

void Helios69Chain::reset()
{
    filter.reset();
    treble.reset();
    mid.reset();
    bass.reset();
    eqCut.reset();
    preLineAmp.reset();
    postLineAmp.reset();
}

void Helios69Chain::setInputTrimDb(float trimDb)
{
    inputGain = ParamMapping::dbToGain(trimDb);
}

void Helios69Chain::setFilterMode(int modeIndex)
{
    filter.setMode(static_cast<FilterHPF::Mode>(modeIndex));
}

void Helios69Chain::setTrGainDb(float gainDb)
{
    treble.setGainDb(gainDb);
}

void Helios69Chain::setMidFreq(int index)
{
    mid.setFreq(static_cast<MidPK::FreqPos>(index));
}

void Helios69Chain::setMidGainDb(float gainDb)
{
    mid.setGainDb(gainDb);
}

void Helios69Chain::setBassFreq(int index)
{
    bass.setFreq(static_cast<BassEQ::FreqPos>(index));
}

void Helios69Chain::setBassGainDb(float gainDb)
{
    bass.setGainDb(gainDb);
}

void Helios69Chain::setBass50Hz(bool enabled)
{
    bass.set50HzCut(enabled);
}

void Helios69Chain::setEqCut(bool enabled)
{
    eqCut.setBypass(enabled);
}

void Helios69Chain::setPostTrimDb(float trimDb)
{
    postLineAmp.setTrimDb(trimDb);
}

void Helios69Chain::setSaturation(bool enabled)
{
    postLineAmp.setSaturation(enabled);
}

float Helios69Chain::processSample(float x)
{
    float y = x * inputGain;

    y = filter.processSample(y);
    y = preLineAmp.processSample(y);

    y = treble.processSample(y);
    y = mid.processSample(y);

    y = bass.processSample(y);
    y = eqCut.processSample(y);
    y = postLineAmp.processSample(y);

    return y;
}
