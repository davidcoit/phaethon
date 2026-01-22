#include "TrebleTR.h"
#include "../../params/ParamMapping.h"

namespace
{
    double freqForPos(TrebleTR::FreqPos pos)
    {
        switch (pos)
        {
            case TrebleTR::FreqPos::Pos2: return 2000.0;
            case TrebleTR::FreqPos::Pos4: return 4000.0;
            case TrebleTR::FreqPos::Pos6: return 6000.0;
            case TrebleTR::FreqPos::Pos8: return 8000.0;
            case TrebleTR::FreqPos::Pos10: return 10000.0;
        }
        return 2000.0;
    }
}

void TrebleTR::ShelfFilter::reset()
{
    filter.reset();
}

void TrebleTR::ShelfFilter::setCoefficients(double sampleRate, double freqHz, double gainDb)
{
    auto coeff = juce::dsp::IIR::Coefficients<double>::makeHighShelf(sampleRate, freqHz, 0.7, ParamMapping::dbToGain(static_cast<float>(gainDb)));
    filter.state = coeff;
}

double TrebleTR::ShelfFilter::process(double x)
{
    return filter.processSample(x);
}

void TrebleTR::prepare(double sampleRate, int fadeMs)
{
    fs = sampleRate;
    freqXfade.reset(sampleRate, fadeMs / 1000.0);
    reset();

    const auto freq = freqForPos(currentFreq);
    cutCurrent.setCoefficients(fs, freq, -maxAbsGainDb);
    boostCurrent.setCoefficients(fs, freq, maxAbsGainDb);
}

void TrebleTR::reset()
{
    cutCurrent.reset();
    cutNext.reset();
    boostCurrent.reset();
    boostNext.reset();
    freqXfade.setCurrentAndTargetValue(0.0f);
    transitioning = false;
}

void TrebleTR::setFreq(FreqPos newFreq)
{
    if (newFreq == currentFreq)
        return;

    beginFreqTransition(newFreq);
}

void TrebleTR::beginFreqTransition(FreqPos newFreq)
{
    pendingFreq = newFreq;
    const auto freq = freqForPos(newFreq);

    cutNext.reset();
    boostNext.reset();
    cutNext.setCoefficients(fs, freq, -maxAbsGainDb);
    boostNext.setCoefficients(fs, freq, maxAbsGainDb);

    freqXfade.reset(fs, 0.02);
    freqXfade.setCurrentAndTargetValue(0.0f);
    freqXfade.setTargetValue(1.0f);
    transitioning = true;
}

void TrebleTR::setGainDb(float newGainDb)
{
    gainDb = newGainDb;
}

float TrebleTR::processSample(float x)
{
    const float gainXfade = ParamMapping::gainDbToXfade(gainDb, maxAbsGainDb);

    double cutOut = 0.0;
    double boostOut = 0.0;

    if (transitioning)
    {
        const float fMix = freqXfade.getNextValue();
        const double cutA = cutCurrent.process(x);
        const double cutB = cutNext.process(x);
        const double boostA = boostCurrent.process(x);
        const double boostB = boostNext.process(x);

        cutOut = (1.0f - fMix) * cutA + fMix * cutB;
        boostOut = (1.0f - fMix) * boostA + fMix * boostB;

        if (!freqXfade.isSmoothing())
        {
            cutCurrent = cutNext;
            boostCurrent = boostNext;
            currentFreq = pendingFreq;
            transitioning = false;
            freqXfade.setCurrentAndTargetValue(0.0f);
        }
    }
    else
    {
        cutOut = cutCurrent.process(x);
        boostOut = boostCurrent.process(x);
    }

    const float y = (1.0f - gainXfade) * static_cast<float>(cutOut)
                    + gainXfade * static_cast<float>(boostOut);

    return y;
}
