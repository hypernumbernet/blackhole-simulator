#pragma once

#include <QObject>
#include <QMap>
#include <QQuaternion>

namespace bhs // Black Hole Simulator
{

enum class Preset {
    Random,
    SunEarth,
    EarthSun,
    EarthMoon,
    SunEarthVenus,
    TestSamePosition,
};

struct SimCondition
{
    int engine = 0;
    Preset preset = Preset::Random;
    float timePerFrame = 1000.0f;
    int numberOfParticles = 4000;
    float massAvg = 6.0e+29f;
    bool massRandom = true;
};

inline float randf() {return (float)rand() / (float)RAND_MAX;}

}

class UpdateUi : public QObject
{
    Q_OBJECT

public:

    static constexpr int SCALE_SLIDER_CENTER = 5000;

    const QMap<int, QString>* const ENGINE;
    const QMap<bhs::Preset, QString>* const PRESET;

    UpdateUi()
        : ENGINE(new QMap<int, QString>{
            {0, tr("Gravity 3D Mass Differential")},
            {1, tr("Gravity 3D Mass Integral")},
            {2, tr("Gravity 3S-Velocity M. Diff.")},
            //{0, tr("Gravity3DIntegral")},
            //{0, tr("Gravity2DDifferential")},
            //{0, tr("Gravity2DIntegral")},
        })
        , PRESET(new QMap<bhs::Preset, QString>{
            {bhs::Preset::Random, tr("Random Cube")},
            {bhs::Preset::SunEarth, tr("Sun Earth")},
            {bhs::Preset::EarthSun, tr("Earth Sun")},
            {bhs::Preset::EarthMoon, tr("Earth Moon")},
            {bhs::Preset::SunEarthVenus, tr("Sun Earth Venus")},
            {bhs::Preset::TestSamePosition, tr("Test Same Position")},
        })
    {
    };

signals:
    void displayNumberOfParticles(int);
    void displayFrameNumber(int);
    void displayFps(int);
    void displayModelScale(float);
    void displayTimePerFrame(float);
    void updateStartButtonText(bool);
    void displayEngineName(const QString&);
    void displayPresetName(const QString&);
    void applyInitialConditions();
    void frameAdvance();
    void resultReady();
    //void allDone();

};
