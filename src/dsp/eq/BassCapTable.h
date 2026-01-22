#pragma once

namespace BassCapTable
{
    struct Entry
    {
        int freqIndex;     // 0..4 in BASS selector order (0,60,120,250,400)
        double freqHz;
        double capF;
    };

    // With L = 20 H, target caps for labeled freqs:
    // 60 Hz  -> 0.352 uF (use 0.33–0.36 uF)
    // 120 Hz -> 0.088 uF (use 0.082–0.10 uF)
    // 250 Hz -> 20.3 nF (use 20 nF)
    // 400 Hz -> 7.9 nF (use 8.2 nF)
    inline constexpr Entry entries[] = {
        { 0, 0.0, 0.0 },
        { 1, 60.0, 0.352e-6 },
        { 2, 120.0, 0.088e-6 },
        { 3, 250.0, 20e-9 },
        { 4, 400.0, 8.2e-9 },
    };

    inline const Entry* findByIndex(int index)
    {
        for (const auto& entry : entries)
            if (entry.freqIndex == index)
                return &entry;
        return nullptr;
    }
}
