#pragma once

#include "abstractnbodyengine.h"
#include "updateui.h"

// Gravity 3D Mass Differential N-Body Engine
class G3DMassDiffNBE : public AbstractNBodyEngine
{
public:

    enum class Preset {
        Random,
        SunEarth,
        EarthSun,
        EarthMoon,
        SunEarthVenus,
        TestSamePosition,
    };

    G3DMassDiffNBE(
            UpdateUi* const,
            quint64 numberOfParticles,
            float timePerFrame,
            Preset presetNumber);
    ~G3DMassDiffNBE();

    void calculateTimeProgress() const override;
    void calculateInteraction() const override;
    void debug() const override;
    QString name() const override;

    void setTimePerFrame(float);

private:
    void initParticlesRandam();
    void initSunEarth();
    void initEarthSun();
    void initEarthMoon();
    void initSunEarthVenus();
    void initTestSamePosition();

    //void calculateDistances();

    // 1フレームあたりの物理計算上の時間 (second)
    float m_timePerFrame;

    // 質量 (kg)
    float* m_mass;
};
