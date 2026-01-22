#include "MidPK.h"
#include "MidTapTable.h"
#include "../../params/ParamMapping.h"

namespace
{
    double freqForPos(MidPK::FreqPos pos)
    {
        switch (pos)
        {
            case MidPK::FreqPos::Pos0_7: return 700.0;
            case MidPK::FreqPos::Pos1_0: return 1000.0;
            case MidPK::FreqPos::Pos1_4: return 1400.0;
            case MidPK::FreqPos::Pos2_0: return 2000.0;
            case MidPK::FreqPos::Pos2_8: return 2800.0;
            case MidPK::FreqPos::Pos3_5: return 3500.0;
            case MidPK::FreqPos::Pos4_5: return 4500.0;
            case MidPK::FreqPos::Pos6_0: return 6000.0;
        }
        return 1000.0;
    }

    const MidTapTable::Entry* tapEntryForPos(MidPK::FreqPos pos)
    {
        return MidTapTable::findByIndex(static_cast<int>(pos));
    }
}

void MidPK::WdfNetwork::prepare(double sampleRate)
{
    fs = sampleRate;
}

void MidPK::WdfNetwork::reset()
{
    cap.reset();
    ind.reset();
}

void MidPK::WdfNetwork::setLC(double inductanceH, double capacitanceF)
{
    ind.prepare(fs, inductanceH);
    cap.prepare(fs, capacitanceF);
}

void MidPK::WdfNetwork::setSeriesR(double rOhms)
{
    seriesR.setResistance(rOhms);
}

double MidPK::WdfNetwork::process(double x)
{
    network.setIncident(x);
    return lcParallel.getVoltage();
}

void MidPK::prepare(double sampleRate, int fadeMs)
{
    fs = sampleRate;
    xfade.reset(sampleRate, fadeMs / 1000.0);
    reset();

    current.prepare(sampleRate);
    next.prepare(sampleRate);

    const float x = ParamMapping::gainDbToXfade(gainDb, 12.0f);
    const double rSeries = 200.0 + (1.0 - x) * 19800.0;
    current.setSeriesR(rSeries);
    next.setSeriesR(rSeries);

    const auto* entry = tapEntryForPos(currentFreq);
    if (entry != nullptr)
        current.setLC(entry->tapInductorH, entry->capF);
}

void MidPK::reset()
{
    current.reset();
    next.reset();
    xfade.setCurrentAndTargetValue(0.0f);
    transitioning = false;
}

void MidPK::setFreq(FreqPos newFreq)
{
    if (newFreq == currentFreq)
        return;

    beginTransition(newFreq);
}

void MidPK::beginTransition(FreqPos newFreq)
{
    pendingFreq = newFreq;
    const auto* entry = tapEntryForPos(newFreq);

    next.reset();
    if (entry != nullptr)
        next.setLC(entry->tapInductorH, entry->capF);

    const float x = ParamMapping::gainDbToXfade(gainDb, 12.0f);
    const double rSeries = 200.0 + (1.0 - x) * 19800.0;
    next.setSeriesR(rSeries);

    xfade.reset(fs, 0.02);
    xfade.setCurrentAndTargetValue(0.0f);
    xfade.setTargetValue(1.0f);
    transitioning = true;
}

void MidPK::setGainDb(float newGainDb)
{
    gainDb = newGainDb;
    const float x = ParamMapping::gainDbToXfade(gainDb, 12.0f);
    const double rSeries = 200.0 + (1.0 - x) * 19800.0;
    current.setSeriesR(rSeries);
    next.setSeriesR(rSeries);
}

float MidPK::processSample(float x)
{
    if (!transitioning)
        return static_cast<float>(current.process(x));

    const float mix = xfade.getNextValue();
    const float yA = static_cast<float>(current.process(x));
    const float yB = static_cast<float>(next.process(x));
    const float y = (1.0f - mix) * yA + mix * yB;

    if (!xfade.isSmoothing())
    {
        current = next;
        currentFreq = pendingFreq;
        transitioning = false;
        xfade.setCurrentAndTargetValue(0.0f);
    }

    return y;
}
