#include "EqCut.h"

void EqCut::prepare(double sampleRate, int fadeMs)
{
    fs = sampleRate;
    xfade.reset(sampleRate, fadeMs / 1000.0);
    constexpr double rShunt = 5100.0;
    constexpr double cShunt = 680e-12;
    const double fc = 1.0 / (2.0 * juce::MathConstants<double>::pi * rShunt * cShunt);
    auto coeff = juce::dsp::IIR::Coefficients<double>::makeLowPass(sampleRate, fc);
    shuntFilter.coefficients = coeff;

    constexpr double rSeriesPad = 2200.0;
    constexpr double rShuntPad = 1300.0;
    const double pad = rShuntPad / (rSeriesPad + rShuntPad);
    const double shuntDivider = rShunt / (rShunt + rShunt);
    padGain = static_cast<float>(pad * shuntDivider);

    reset();
}

void EqCut::reset()
{
    shuntFilter.reset();
    xfade.setCurrentAndTargetValue(0.0f);
    transitioning = false;
}

void EqCut::setBypass(bool bypassed)
{
    if (bypassed == bypass)
        return;

    bypass = bypassed;
    xfade.reset(fs, 0.02);
    xfade.setCurrentAndTargetValue(0.0f);
    xfade.setTargetValue(1.0f);
    transitioning = true;
}

float EqCut::processSample(float x)
{
    const float processed = static_cast<float>(shuntFilter.processSample(x)) * padGain;

    if (!transitioning)
        return bypass ? x : processed;

    const float mix = xfade.getNextValue();
    const float yA = x;
    const float yB = processed;
    const float y = (1.0f - mix) * yA + mix * yB;

    if (!xfade.isSmoothing())
    {
        transitioning = false;
        xfade.setCurrentAndTargetValue(0.0f);
    }

    return y;
}
