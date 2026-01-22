cmake_minimum_required(VERSION 3.22)
project(Helios69WDF VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    juce
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 7.0.12
)
FetchContent_MakeAvailable(juce)

juce_add_plugin(Helios69WDF
    COMPANY_NAME "Phaethon"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    VST3_CATEGORIES "Fx"
    PLUGIN_MANUFACTURER_CODE Phth
    PLUGIN_CODE H69Q
    FORMATS VST3
    PRODUCT_NAME "Helios69WDF"
)

juce_generate_juce_header(Helios69WDF)

target_sources(Helios69WDF
    PRIVATE
        src/PluginProcessor.cpp
        src/PluginProcessor.h
        src/PluginEditor.cpp
        src/PluginEditor.h
        src/params/ParamIDs.h
        src/params/ParamLayout.h
        src/params/ParamMapping.h
        src/dsp/Helios69Chain.cpp
        src/dsp/Helios69Chain.h
        src/dsp/utils/Smoothing.h
        src/dsp/utils/Denormals.h
        src/dsp/eq/FilterHPF.cpp
        src/dsp/eq/FilterHPF.h
        src/dsp/eq/TrebleTR.cpp
        src/dsp/eq/TrebleTR.h
        src/dsp/eq/MidPK.cpp
        src/dsp/eq/MidPK.h
        src/dsp/eq/MidTapTable.h
        src/dsp/eq/BassEQ.cpp
        src/dsp/eq/BassEQ.h
        src/dsp/eq/BassCapTable.h
        src/dsp/eq/EqCut.cpp
        src/dsp/eq/EqCut.h
        src/dsp/line_amp/LineAmpPre22113.cpp
        src/dsp/line_amp/LineAmpPre22113.h
        src/dsp/line_amp/LineAmpPost22113.cpp
        src/dsp/line_amp/LineAmpPost22113.h
        src/dsp/wdf/WdfElements.h
)

target_compile_definitions(Helios69WDF
    PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(Helios69WDF
    PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_warning_flags
        juce::juce_recommended_lto_flags
)
