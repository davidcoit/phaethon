#pragma once

#include <algorithm>
#include <cmath>

namespace ParamMapping
{
    inline float gainDbToXfade(float gainDb, float maxAbsDb)
    {
        const float clamped = std::max(-maxAbsDb, std::min(maxAbsDb, gainDb));
        return (clamped + maxAbsDb) / (2.0f * maxAbsDb);
    }

    inline float dbToGain(float db)
    {
        return std::pow(10.0f, db / 20.0f);
    }
}
