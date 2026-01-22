#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginProcessor.h"

class Helios69AudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit Helios69AudioProcessorEditor(Helios69AudioProcessor&);
    ~Helios69AudioProcessorEditor() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    Helios69AudioProcessor& processor;
    std::unique_ptr<juce::GenericAudioProcessorEditor> genericEditor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Helios69AudioProcessorEditor)
};
