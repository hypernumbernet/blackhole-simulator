#pragma once

#include "abstractnbodyengine.h"
#include "updateui.h"
#include "initializer3d.h"

// Gravity 3D Mass Differential N-Body Engine
class G3DMassDiffNBE : public AbstractNBodyEngine, Initializer3D
{
public:
    G3DMassDiffNBE(
            UpdateUi* const,
            quint64 numberOfParticles,
            float timePerFrame,
            Preset presetNumber);
    ~G3DMassDiffNBE();

    void calculateTimeProgress() const override;
    void calculateInteraction() const override;
    void debug() const override;

    void setTimePerFrame(float);

private:

    // Physically calculated time per frame (second)
    float m_timePerFrame;
};
