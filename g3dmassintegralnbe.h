#pragma once

#include "abstractnbodyengine.h"
#include "updateui.h"

// Gravity 3D Mass Integral N-Body Engine
class G3DMassIntegralNBE : public AbstractNBodyEngine
{
    struct Distance {
        float invR;
        float unitX;
        float unitY;
        float unitZ;
    };

public:
    enum class Preset {
        Random,
        SunEarth,
        EarthSun,
        EarthMoon,
        SunEarthVenus,
        TestSamePosition,
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
    //QString name() const override;

    void setTimePerFrame(float);
    bool calculateDistance(Distance&, quint64, quint64) const;

private:

    void initParticlesRandam();
    void initSunEarth();
    void initEarthSun();
    void initEarthMoon();
    void initSunEarthVenus();
    void initTestSamePosition();
    void calculateDistances();

    // Physically calculated time per frame (second)
    float m_timePerFrame;

    // Stores the mass of each particle (kg)
    float* m_mass;

    // Stores the reciprocal of the previously calculated distance.
    float* m_inversedDistances;
};
