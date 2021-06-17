#pragma once

#include "abstractnbodyengine.h"
#include "updateui.h"

class Gravity3DMassDifferentialNBodyEngine : public AbstractNBodyEngine
{
public:

    enum class Preset {
        Random,
        SunEarth,
        EarthSun,
        EarthMoon,
        TestSamePosition,
    };

    Gravity3DMassDifferentialNBodyEngine(
            UpdateUi* const,
            quint64 numberOfParticles,
            float timePerFrame,
            Preset presetNumber);
    ~Gravity3DMassDifferentialNBodyEngine();

    //void newParticles();
    float calculateDistance(quint64, quint64) const;
    void timeProgress() const;
    void calculateInteraction() const;
    void debug() const;
    quint64 numberOfParticle() const;
    float* coordinates() const;
    float modelScale() const;

    void setTimePerFrame(float);

private:
    void initParticlesRandam();
    void initSunEarth();
    void initEarthSun();
    void initEarthMoon();
    void initTestSamePosition();

    //void calculateDistances();

    // 1フレームあたりの物理計算上の時間 (second)
    float m_timePerFrame;

    // 質量 (kg)
    float* m_mass;

    // 前回フレームの距離の逆数
    //float* inversedDistances;
};
