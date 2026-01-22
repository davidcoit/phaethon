#pragma once

namespace MidTapTable
{
    enum class Tap
    {
        Tap1,
        Tap2,
        Tap3,
        Tap4
    };

    struct Entry
    {
        int freqIndex;      // 0..7 in MID selector order
        double freqHz;
        double capF;
        double tapInductorH;
        Tap tap;
    };

    // Derived from provided tapped-inductor groupings:
    // Tap 1: 0.663 H
    // Tap 2: 0.366 H
    // Tap 3: 0.186 H
    // Tap 4: 0.108 H
    inline constexpr Entry entries[] = {
        { 0, 700.0, 78e-9, 0.663, Tap::Tap1 },
        { 1, 1000.0, 68e-9, 0.366, Tap::Tap2 },
        { 2, 1400.0, 36e-9, 0.366, Tap::Tap2 },
        { 3, 2000.0, 33e-9, 0.186, Tap::Tap3 },
        { 4, 2800.0, 18e-9, 0.186, Tap::Tap3 },
        { 5, 3500.0, 18e-9, 0.108, Tap::Tap4 },
        { 6, 4500.0, 12e-9, 0.108, Tap::Tap4 },
        { 7, 6000.0, 6.8e-9, 0.108, Tap::Tap4 },
    };

    inline const Entry* findByIndex(int index)
    {
        for (const auto& entry : entries)
            if (entry.freqIndex == index)
                return &entry;
        return nullptr;
    }
}
