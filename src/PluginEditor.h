#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginProcessor.h"
#include "params/ParamIDs.h"

class Helios69AudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit Helios69AudioProcessorEditor(Helios69AudioProcessor&);
    ~Helios69AudioProcessorEditor() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    Helios69AudioProcessor& processor;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;

    juce::Slider inputSlider;
    juce::Slider bassFreqSlider;
    juce::Slider midFreqSlider;
    juce::Slider midGainSlider;
    juce::ToggleButton midModeToggle;
    juce::Slider highGainSlider;
    juce::Slider outputSlider;

    juce::Label inputLabel;
    juce::Label bassLabel;
    juce::Label midFreqLabel;
    juce::Label midGainLabel;
    juce::Label midModeLabel;
    juce::Label highLabel;
    juce::Label outputLabel;

    std::unique_ptr<SliderAttachment> inputAttachment;
    std::unique_ptr<SliderAttachment> bassFreqAttachment;
    std::unique_ptr<SliderAttachment> midFreqAttachment;
    std::unique_ptr<SliderAttachment> midGainAttachment;
    std::unique_ptr<ButtonAttachment> midModeAttachment;
    std::unique_ptr<SliderAttachment> highGainAttachment;
    std::unique_ptr<SliderAttachment> outputAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Helios69AudioProcessorEditor)
};
