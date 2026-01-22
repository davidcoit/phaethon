#include "BassEQ.h"
#include "BassCapTable.h"
#include "../../params/ParamMapping.h"

namespace
{
    constexpr double bassInductorH = 20.0;
}

void BassEQ::WdfNetwork::prepare(double sampleRate)
{
    fs = sampleRate;
}

void BassEQ::WdfNetwork::reset()
{
    cap.reset();
    ind.reset();
}

void BassEQ::WdfNetwork::setLC(double inductanceH, double capacitanceF)
{
    ind.prepare(fs, inductanceH);
    cap.prepare(fs, capacitanceF);
}

void BassEQ::WdfNetwork::setSeriesR(double rOhms)
{
    seriesR.setResistance(rOhms);
}

double BassEQ::WdfNetwork::process(double x)
{
    network.setIncident(x);
    return lcParallel.getVoltage();
}

void BassEQ::HPFilter::reset()
{
    filter.reset();
}

void BassEQ::HPFilter::setCoefficients(double sampleRate, double freqHz)
{
    auto coeff = juce::dsp::IIR::Coefficients<double>::makeHighPass(sampleRate, freqHz);
    filter.coefficients = coeff;
}

double BassEQ::HPFilter::process(double x)
{
    return filter.processSample(x);
}

void BassEQ::prepare(double sampleRate, int fadeMs)
{
    fs = sampleRate;
    freqXfade.reset(sampleRate, fadeMs / 1000.0);
    cut50Xfade.reset(sampleRate, fadeMs / 1000.0);
    gainXfade.reset(sampleRate, 0.02);
    reset();

    boostCurrent.prepare(sampleRate);
    boostNext.prepare(sampleRate);
    cutCurrent.prepare(sampleRate);
    cutNext.prepare(sampleRate);

    const float x = ParamMapping::gainDbToXfade(gainDb, 12.0f);
    gainXfade.setCurrentAndTargetValue(x);

    const double boostR = 200.0 + (1.0 - x) * 4800.0;
    const double cutR = 5000.0 + (1.0 - x) * 45000.0;
    boostCurrent.setSeriesR(boostR);
    boostNext.setSeriesR(boostR);
    cutCurrent.setSeriesR(cutR);
    cutNext.setSeriesR(cutR);

    const auto* entry = BassCapTable::findByIndex(static_cast<int>(currentFreq));
    if (entry != nullptr && entry->capF > 0.0)
    {
        boostCurrent.setLC(bassInductorH, entry->capF);
        cutCurrent.setLC(bassInductorH, entry->capF);
    }

    cut50Current.setCoefficients(fs, 50.0);
}

void BassEQ::reset()
{
    boostCurrent.reset();
    boostNext.reset();
    cutCurrent.reset();
    cutNext.reset();
    cut50Current.reset();
    freqXfade.setCurrentAndTargetValue(0.0f);
    cut50Xfade.setCurrentAndTargetValue(0.0f);
    gainXfade.setCurrentAndTargetValue(0.5f);
    transitioning = false;
}

void BassEQ::setFreq(FreqPos newFreq)
{
    if (newFreq == currentFreq)
        return;

    beginTransition(newFreq);
}

void BassEQ::beginTransition(FreqPos newFreq)
{
    pendingFreq = newFreq;
    const auto* entry = BassCapTable::findByIndex(static_cast<int>(newFreq));

    boostNext.reset();
    cutNext.reset();
    if (entry != nullptr && entry->capF > 0.0)
    {
        boostNext.setLC(bassInductorH, entry->capF);
        cutNext.setLC(bassInductorH, entry->capF);
    }

    const float x = ParamMapping::gainDbToXfade(gainDb, 12.0f);
    const double boostR = 200.0 + (1.0 - x) * 4800.0;
    const double cutR = 5000.0 + (1.0 - x) * 45000.0;
    boostNext.setSeriesR(boostR);
    cutNext.setSeriesR(cutR);

    freqXfade.reset(fs, 0.02);
    freqXfade.setCurrentAndTargetValue(0.0f);
    freqXfade.setTargetValue(1.0f);
    transitioning = true;
}

void BassEQ::setGainDb(float newGainDb)
{
    gainDb = newGainDb;
    const float x = ParamMapping::gainDbToXfade(gainDb, 12.0f);
    gainXfade.setTargetValue(x);

    const double boostR = 200.0 + (1.0 - x) * 4800.0;
    const double cutR = 5000.0 + (1.0 - x) * 45000.0;
    boostCurrent.setSeriesR(boostR);
    boostNext.setSeriesR(boostR);
    cutCurrent.setSeriesR(cutR);
    cutNext.setSeriesR(cutR);
}

void BassEQ::set50HzCut(bool enabled)
{
    if (enabled == cut50Enabled)
        return;

    cut50Enabled = enabled;
    cut50Xfade.reset(fs, 0.02);
    cut50Xfade.setTargetValue(cut50Enabled ? 1.0f : 0.0f);
}

float BassEQ::processSample(float x)
{
    float y = x;
    const float gainMix = gainXfade.getNextValue();

    float boostOut = 0.0f;
    float cutBandOut = 0.0f;

    if (transitioning)
    {
        const float mix = freqXfade.getNextValue();
        const float boostA = static_cast<float>(boostCurrent.process(y));
        const float boostB = static_cast<float>(boostNext.process(y));
        const float cutA = static_cast<float>(cutCurrent.process(y));
        const float cutB = static_cast<float>(cutNext.process(y));
        boostOut = (1.0f - mix) * boostA + mix * boostB;
        cutBandOut = (1.0f - mix) * cutA + mix * cutB;

        if (!freqXfade.isSmoothing())
        {
            currentFreq = pendingFreq;
            boostCurrent.reset();
            cutCurrent.reset();
            const auto* entry = BassCapTable::findByIndex(static_cast<int>(currentFreq));
            if (entry != nullptr && entry->capF > 0.0)
            {
                boostCurrent.setLC(bassInductorH, entry->capF);
                cutCurrent.setLC(bassInductorH, entry->capF);
            }
            transitioning = false;
            freqXfade.setCurrentAndTargetValue(0.0f);
        }
    }
    else
    {
        boostOut = static_cast<float>(boostCurrent.process(y));
        cutBandOut = static_cast<float>(cutCurrent.process(y));
    }

    y = (1.0f - gainMix) * cutBandOut + gainMix * boostOut;

    const float cutMix = cut50Xfade.getNextValue();
    if (cutMix > 0.0f)
    {
        const float cutHpOut = static_cast<float>(cut50Current.process(y));
        y = (1.0f - cutMix) * y + cutMix * cutHpOut;
    }

    return y;
}
