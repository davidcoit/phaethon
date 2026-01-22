#include "FilterHPF.h"

void FilterHPF::TwoPoleHPF::reset()
{
    f1.reset();
    f2.reset();
}

void FilterHPF::TwoPoleHPF::setCutoff(double sampleRate, double hz)
{
    auto coeff = juce::dsp::IIR::Coefficients<double>::makeHighPass(sampleRate, hz);
    f1.coefficients = coeff;
    f2.coefficients = coeff;
}

double FilterHPF::TwoPoleHPF::process(double x)
{
    return f2.processSample(f1.processSample(x));
}

void FilterHPF::prepare(double sampleRate, int fadeMs)
{
    fs = sampleRate;
    fadeSamples = static_cast<int>((fadeMs / 1000.0) * fs);
    xfade.reset(sampleRate, fadeMs / 1000.0);
    reset();
}

void FilterHPF::reset()
{
    current.reset();
    next.reset();
    xfade.setCurrentAndTargetValue(0.0f);
    transitioning = false;
}

void FilterHPF::setMode(Mode newMode)
{
    if (newMode == currentMode)
        return;

    beginTransition(newMode);
}

void FilterHPF::beginTransition(Mode newMode)
{
    pendingMode = newMode;
    next.reset();

    if (pendingMode == Mode::Hz80)
        next.setCutoff(fs, 80.0);
    else if (pendingMode == Mode::Hz140)
        next.setCutoff(fs, 140.0);

    xfade.reset(fs, 0.02);
    xfade.setCurrentAndTargetValue(0.0f);
    xfade.setTargetValue(1.0f);
    transitioning = true;
}

float FilterHPF::processSample(float x)
{
    if (currentMode == Mode::Off && !transitioning)
        return x;

    if (!transitioning)
        return static_cast<float>(current.process(x));

    const float mix = xfade.getNextValue();
    const float yA = (currentMode == Mode::Off) ? x : static_cast<float>(current.process(x));
    const float yB = (pendingMode == Mode::Off) ? x : static_cast<float>(next.process(x));
    const float y = (1.0f - mix) * yA + mix * yB;

    if (!xfade.isSmoothing())
    {
        current.reset();
        if (pendingMode == Mode::Hz80)
            current.setCutoff(fs, 80.0);
        else if (pendingMode == Mode::Hz140)
            current.setCutoff(fs, 140.0);

        currentMode = pendingMode;
        transitioning = false;
        xfade.setCurrentAndTargetValue(0.0f);
    }

    return y;
}
