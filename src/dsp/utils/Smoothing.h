#pragma once

#include <juce_dsp/juce_dsp.h>

namespace Smoothing
{
    inline juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> makeLinear(float initial)
    {
        juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> v;
        v.setCurrentAndTargetValue(initial);
        return v;
    }
}
