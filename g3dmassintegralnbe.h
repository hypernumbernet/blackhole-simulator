#pragma once

#include "abstractnbodyengine.h"
#include "updateui.h"
#include "initializer3d.h"

// Gravity 3D Mass Integral N-Body Engine
class G3DMassIntegralNBE : public AbstractNBodyEngine, Initializer3D
{
    struct Distance {
        float invR;
        float unitX;
        float unitY;
        float unitZ;
    };

public:
    G3DMassIntegralNBE(
            UpdateUi* const,
            quint64 numberOfParticles,
            float timePerFrame,
            Preset presetNumber);
    ~G3DMassIntegralNBE();

    void calculateTimeProgress() const override;
    void calculateInteraction() const override;
    void debug() const override;

    void setTimePerFrame(float);
    bool calculateDistance(Distance&, quint64, quint64) const;

private:
    void calculateDistances();

    // Physically calculated time per frame (second)
    float m_timePerFrame;

    // Stores the reciprocal of the previously calculated distance.
    float* m_inversedDistances;
};
