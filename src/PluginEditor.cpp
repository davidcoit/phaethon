#include "PluginEditor.h"

Helios69AudioProcessorEditor::Helios69AudioProcessorEditor(Helios69AudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    genericEditor = std::make_unique<juce::GenericAudioProcessorEditor>(processor);
    addAndMakeVisible(*genericEditor);
    setSize(640, 420);
}

void Helios69AudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

void Helios69AudioProcessorEditor::resized()
{
    if (genericEditor)
        genericEditor->setBounds(getLocalBounds());
}
