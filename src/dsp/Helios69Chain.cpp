#include "Helios69Chain.h"

void Helios69Chain::prepare(double sampleRate, int samplesPerBlock)
{
    fs = sampleRate;
    filter.prepare(sampleRate, 20);
    treble.prepare(sampleRate, 20);
    mid.prepare(sampleRate, 20);
    bass.prepare(sampleRate, 20);
    eqCut.prepare(sampleRate, 20);
    preLineAmp.prepare(sampleRate);
    postLineAmp.prepare(sampleRate);

    trPkXfade.reset(sampleRate, 0.02);
    trPkXfade.setCurrentAndTargetValue(0.0f);

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

void Helios69Chain::setFilterMode(int modeIndex)
{
    filter.setMode(static_cast<FilterHPF::Mode>(modeIndex));
}

void Helios69Chain::setTrPkMode(int modeIndex)
{
    const bool newTrMode = (modeIndex == 0);
    if (newTrMode == trMode)
        return;

    trMode = newTrMode;
    trPkXfade.reset(fs, 0.02);
    trPkXfade.setCurrentAndTargetValue(0.0f);
    trPkXfade.setTargetValue(1.0f);
}

void Helios69Chain::setTrFreq(int index)
{
    treble.setFreq(static_cast<TrebleTR::FreqPos>(index));
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

void Helios69Chain::setPreDriveDb(float driveDb)
{
    preLineAmp.setDriveDb(driveDb);
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
    float y = x;

    y = filter.processSample(y);
    y = preLineAmp.processSample(y);

    const float trOut = treble.processSample(y);
    const float pkOut = mid.processSample(y);

    if (trPkXfade.isSmoothing())
    {
        const float mix = trPkXfade.getNextValue();
        y = trMode ? (1.0f - mix) * pkOut + mix * trOut
                   : (1.0f - mix) * trOut + mix * pkOut;
    }
    else
    {
        y = trMode ? trOut : pkOut;
    }

    y = bass.processSample(y);
    y = eqCut.processSample(y);
    y = postLineAmp.processSample(y);

    return y;
}
