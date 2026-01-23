#include "PluginEditor.h"
#include <array>
#include <cmath>

namespace
{
    void configureRotary(juce::Slider& slider)
    {
        slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 18);
    }
}

Helios69AudioProcessorEditor::Helios69AudioProcessorEditor(Helios69AudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    configureRotary(inputSlider);
    configureRotary(bassFreqSlider);
    configureRotary(midFreqSlider);
    configureRotary(midGainSlider);
    configureRotary(highGainSlider);
    configureRotary(outputSlider);

    bassFreqSlider.setRange(0.0, 8.0, 1.0);
    bassFreqSlider.textFromValueFunction = [](double v)
    {
        static const char* labels[] = { "C400", "C250", "C120", "C60", "OFF", "B60", "B120", "B250", "B400" };
        const int idx = juce::jlimit(0, 8, static_cast<int>(std::round(v)));
        return juce::String(labels[idx]);
    };

    midFreqSlider.setRange(0.0, 7.0, 1.0);
    midFreqSlider.textFromValueFunction = [](double v)
    {
        static const char* labels[] = { "0.7", "1.0", "1.4", "2.0", "2.8", "3.5", "4.5", "6.0" };
        const int idx = juce::jlimit(0, 7, static_cast<int>(std::round(v)));
        return juce::String(labels[idx]) + "k";
    };

    midModeToggle.setButtonText("Peak");

    inputLabel.setText("Input", juce::dontSendNotification);
    bassLabel.setText("Bass Freq", juce::dontSendNotification);
    midFreqLabel.setText("Mid Freq", juce::dontSendNotification);
    midGainLabel.setText("Mid Gain", juce::dontSendNotification);
    midModeLabel.setText("Peak/Trough", juce::dontSendNotification);
    highLabel.setText("High 10k", juce::dontSendNotification);
    outputLabel.setText("Output", juce::dontSendNotification);

    const std::array<juce::Component*, 14> components = {
        &inputSlider, &bassFreqSlider, &midFreqSlider, &midGainSlider, &midModeToggle, &highGainSlider, &outputSlider,
        &inputLabel, &bassLabel, &midFreqLabel, &midGainLabel, &midModeLabel, &highLabel, &outputLabel
    };
    for (auto* comp : components)
        addAndMakeVisible(comp);

    inputAttachment = std::make_unique<SliderAttachment>(processor.apvts, ParamIDs::inputTrim, inputSlider);
    bassFreqAttachment = std::make_unique<SliderAttachment>(processor.apvts, ParamIDs::bassMode, bassFreqSlider);
    midFreqAttachment = std::make_unique<SliderAttachment>(processor.apvts, ParamIDs::midFreq, midFreqSlider);
    midGainAttachment = std::make_unique<SliderAttachment>(processor.apvts, ParamIDs::midGain, midGainSlider);
    midModeAttachment = std::make_unique<ButtonAttachment>(processor.apvts, ParamIDs::midMode, midModeToggle);
    highGainAttachment = std::make_unique<SliderAttachment>(processor.apvts, ParamIDs::trGain, highGainSlider);
    outputAttachment = std::make_unique<SliderAttachment>(processor.apvts, ParamIDs::postTrim, outputSlider);

    setSize(760, 380);
}

void Helios69AudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.setFont(14.0f);
}

void Helios69AudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(20);
    auto topRow = area.removeFromTop(140);
    auto bottomRow = area.removeFromTop(140);

    auto placeBlock = [](juce::Rectangle<int> bounds, juce::Label& label, juce::Component& control)
    {
        auto labelArea = bounds.removeFromTop(20);
        label.setBounds(labelArea);
        control.setBounds(bounds);
    };

    auto blockW = topRow.getWidth() / 4;
    placeBlock(topRow.removeFromLeft(blockW), inputLabel, inputSlider);
    placeBlock(topRow.removeFromLeft(blockW), bassLabel, bassFreqSlider);
    placeBlock(topRow.removeFromLeft(blockW), midFreqLabel, midFreqSlider);
    placeBlock(topRow, midGainLabel, midGainSlider);

    blockW = bottomRow.getWidth() / 3;
    auto midModeArea = bottomRow.removeFromLeft(blockW);
    midModeLabel.setBounds(midModeArea.removeFromTop(20));
    midModeToggle.setBounds(midModeArea.removeFromTop(24));

    placeBlock(bottomRow.removeFromLeft(blockW), highLabel, highGainSlider);
    placeBlock(bottomRow, outputLabel, outputSlider);
}
