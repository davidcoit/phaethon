#pragma once

namespace ParamIDs
{
    inline constexpr const char* filterMode = "filterMode"; // 0/80/140
    inline constexpr const char* trPkMode = "trPkMode";     // TR/PK
    inline constexpr const char* trFreq = "trFreq";         // 2/4/6/8/10
    inline constexpr const char* trGain = "trGain";         // dB
    inline constexpr const char* midFreq = "midFreq";       // 0.7..6k
    inline constexpr const char* midGain = "midGain";       // dB
    inline constexpr const char* bassFreq = "bassFreq";     // 0/60/120/250/400
    inline constexpr const char* bassGain = "bassGain";     // dB
    inline constexpr const char* bass50Hz = "bass50Hz";     // -dB 50 mode
    inline constexpr const char* eqCut = "eqCut";           // bypass
    inline constexpr const char* preDrive = "preDrive";     // dB
    inline constexpr const char* postTrim = "postTrim";     // dB
    inline constexpr const char* saturation = "saturation"; // bool
}
