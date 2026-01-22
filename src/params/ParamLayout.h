#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "ParamIDs.h"

namespace ParamLayout
{
    inline juce::AudioProcessorValueTreeState::ParameterLayout create()
    {
        using namespace juce;
        std::vector<std::unique_ptr<RangedAudioParameter>> params;

        params.push_back(std::make_unique<AudioParameterChoice>(
            ParamIDs::filterMode,
            "Filter",
            StringArray { "OFF", "80", "140" },
            0));

        params.push_back(std::make_unique<AudioParameterChoice>(
            ParamIDs::trPkMode,
            "TR/PK",
            StringArray { "TR", "PK" },
            0));

        params.push_back(std::make_unique<AudioParameterChoice>(
            ParamIDs::trFreq,
            "TR Freq",
            StringArray { "2", "4", "6", "8", "10" },
            0));

        params.push_back(std::make_unique<AudioParameterFloat>(
            ParamIDs::trGain,
            "TR Gain",
            NormalisableRange<float>(-12.0f, 12.0f, 0.01f),
            0.0f));

        params.push_back(std::make_unique<AudioParameterChoice>(
            ParamIDs::midFreq,
            "MID Freq",
            StringArray { "0.7", "1.0", "1.4", "2.0", "2.8", "3.5", "4.5", "6.0" },
            1));

        params.push_back(std::make_unique<AudioParameterFloat>(
            ParamIDs::midGain,
            "MID Gain",
            NormalisableRange<float>(-12.0f, 12.0f, 0.01f),
            0.0f));

        params.push_back(std::make_unique<AudioParameterChoice>(
            ParamIDs::bassFreq,
            "BASS Freq",
            StringArray { "0", "60", "120", "250", "400" },
            0));

        params.push_back(std::make_unique<AudioParameterFloat>(
            ParamIDs::bassGain,
            "BASS Gain",
            NormalisableRange<float>(-12.0f, 12.0f, 0.01f),
            0.0f));

        params.push_back(std::make_unique<AudioParameterBool>(
            ParamIDs::bass50Hz,
            "-dB 50",
            false));

        params.push_back(std::make_unique<AudioParameterBool>(
            ParamIDs::eqCut,
            "EQ CUT",
            false));

        params.push_back(std::make_unique<AudioParameterFloat>(
            ParamIDs::preDrive,
            "PRE",
            NormalisableRange<float>(-6.0f, 12.0f, 0.01f),
            0.0f));

        params.push_back(std::make_unique<AudioParameterFloat>(
            ParamIDs::postTrim,
            "POST",
            NormalisableRange<float>(-24.0f, 12.0f, 0.01f),
            0.0f));

        params.push_back(std::make_unique<AudioParameterBool>(
            ParamIDs::saturation,
            "Saturation",
            false));

        return { params.begin(), params.end() };
    }
}
