#pragma once

namespace ParamIDs
{
    inline constexpr const char* filterMode = "filterMode"; // 0/80/140
    inline constexpr const char* trGain = "trGain";         // dB
    inline constexpr const char* midFreq = "midFreq";       // 0.7..6k
    inline constexpr const char* midGain = "midGain";       // dB
    inline constexpr const char* midMode = "midMode";       // peak/trough
    inline constexpr const char* bassMode = "bassMode";     // cut/boost freq selector
    inline constexpr const char* bass50Hz = "bass50Hz";     // -dB 50 mode
    inline constexpr const char* eqCut = "eqCut";           // bypass
    inline constexpr const char* inputTrim = "inputTrim";   // dB
    inline constexpr const char* postTrim = "postTrim";     // dB
    inline constexpr const char* saturation = "saturation"; // bool
}
