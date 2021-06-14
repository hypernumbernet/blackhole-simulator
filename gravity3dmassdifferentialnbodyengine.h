#pragma once

#include "abstractnbodyengine.h"

class Gravity3DMassDifferentialNBodyEngine : public AbstractNBodyEngine
{
public:

    enum class Preset {
        Random,
        SunEarth,
        EarthSun,
    };

    Gravity3DMassDifferentialNBodyEngine(quint64 numberOfParticles,
                                         float timePerFrame = 3600.0f,
                                         Preset presetNumber = Preset::Random);
    ~Gravity3DMassDifferentialNBodyEngine();

    //void newParticles();
    float getDistance(quint64, quint64);
    void timeProgress();
    void calculateInteraction();
    void debug();
    quint64 getNumberOfParticle();
    float* getCoordinates();
    float getModelScale();

private:
    void initParticlesRandam();
    void initSunEarth();
    void initEarthSun();

    //void calculateDistances();

    // 1フレームあたりの物理計算上の時間 (second)
    float timePerFrame;

    // 質量 (kg)
    float* mass;

    // 前回フレームの距離の逆数
    //float* inversedDistances;
};
