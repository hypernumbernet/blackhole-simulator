#pragma once

#include "abstractnbodyengine.h"
#include "updateui.h"
#include "initializer3d.h"
#include "quaternion.h"

using namespace bhs;

// Gravity 3D-Coordinate 3S-Velocity Mass Differential N-Body Engine
class G3SVMassDiffNBE : public AbstractNBodyEngine, private Initializer3D
{
public:
    G3SVMassDiffNBE(UpdateUi* const, const bhs::SimCondition&);
    ~G3SVMassDiffNBE();

    void calculateTimeProgress() const override;
    void calculateInteraction() const override;
    void debug() const override;

    void setTimePerFrame(float);

private:

    // Physically calculated time per frame (second)
    float m_timePerFrame;

};
