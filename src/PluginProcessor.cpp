#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "params/ParamIDs.h"
#include "dsp/utils/Denormals.h"

Helios69AudioProcessor::Helios69AudioProcessor()
    : AudioProcessor(BusesProperties().withInput("Input", juce::AudioChannelSet::stereo(), true)
                                       .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, "PARAMS", ParamLayout::create())
{
}

const juce::String Helios69AudioProcessor::getName() const { return "Helios69WDF"; }

bool Helios69AudioProcessor::acceptsMidi() const { return false; }
bool Helios69AudioProcessor::producesMidi() const { return false; }
bool Helios69AudioProcessor::isMidiEffect() const { return false; }

double Helios69AudioProcessor::getTailLengthSeconds() const { return 0.0; }

int Helios69AudioProcessor::getNumPrograms() { return 1; }
int Helios69AudioProcessor::getCurrentProgram() { return 0; }
void Helios69AudioProcessor::setCurrentProgram(int) {}
const juce::String Helios69AudioProcessor::getProgramName(int) { return {}; }
void Helios69AudioProcessor::changeProgramName(int, const juce::String&) {}

void Helios69AudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    chain.prepare(sampleRate, samplesPerBlock);
}

void Helios69AudioProcessor::releaseResources()
{
}

void Helios69AudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    Denormals::ScopedNoDenormals noDenormals;

    const auto totalNumInputChannels = getTotalNumInputChannels();
    const auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    const auto* filterMode = apvts.getRawParameterValue(ParamIDs::filterMode);
    const auto* trGain = apvts.getRawParameterValue(ParamIDs::trGain);
    const auto* midFreq = apvts.getRawParameterValue(ParamIDs::midFreq);
    const auto* midGain = apvts.getRawParameterValue(ParamIDs::midGain);
    const auto* midMode = apvts.getRawParameterValue(ParamIDs::midMode);
    const auto* bassMode = apvts.getRawParameterValue(ParamIDs::bassMode);
    const auto* bass50Hz = apvts.getRawParameterValue(ParamIDs::bass50Hz);
    const auto* eqCut = apvts.getRawParameterValue(ParamIDs::eqCut);
    const auto* inputTrim = apvts.getRawParameterValue(ParamIDs::inputTrim);
    const auto* postTrim = apvts.getRawParameterValue(ParamIDs::postTrim);
    const auto* saturation = apvts.getRawParameterValue(ParamIDs::saturation);

    chain.setInputTrimDb(inputTrim->load());
    chain.setFilterMode(static_cast<int>(filterMode->load()));
    chain.setTrGainDb(trGain->load());
    chain.setMidFreq(static_cast<int>(midFreq->load()));

    const bool isPeak = midMode->load() > 0.5f;
    const float midGainAbs = midGain->load();
    chain.setMidGainDb(isPeak ? midGainAbs : -midGainAbs);

    const int bassModeIndex = static_cast<int>(bassMode->load());
    int bassFreqIndex = 0;
    float bassGainDb = 0.0f;
    if (bassModeIndex <= 3)
    {
        bassFreqIndex = 4 - bassModeIndex; // Cut 400..60 maps to 4..1
        bassGainDb = -12.0f;
    }
    else if (bassModeIndex == 4)
    {
        bassFreqIndex = 0;
        bassGainDb = 0.0f;
    }
    else
    {
        bassFreqIndex = bassModeIndex - 4; // Boost 60..400 maps to 1..4
        bassGainDb = 12.0f;
    }

    chain.setBassFreq(bassFreqIndex);
    chain.setBassGainDb(bassGainDb);
    chain.setBass50Hz(bass50Hz->load() > 0.5f);
    chain.setEqCut(eqCut->load() > 0.5f);
    chain.setPostTrimDb(postTrim->load());
    chain.setSaturation(saturation->load() > 0.5f);

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
            channelData[sample] = chain.processSample(channelData[sample]);
    }
}

bool Helios69AudioProcessor::hasEditor() const { return true; }

juce::AudioProcessorEditor* Helios69AudioProcessor::createEditor()
{
    return new Helios69AudioProcessorEditor(*this);
}

void Helios69AudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void Helios69AudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState != nullptr)
        if (xmlState->hasTagName(apvts.state.getType()))
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Helios69AudioProcessor();
}
