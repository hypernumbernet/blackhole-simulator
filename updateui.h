#pragma once

#include <QObject>
#include <QMap>
#include <QMutex>
#include <QListWidget>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QRadioButton>

namespace bhs // Black Hole Simulator
{

enum class Preset {
    RandomCube,
    RandomSphere,
    SunEarth,
    EarthSun,
    EarthMoon,
    SunEarthVenus,
    TestSamePosition,
};

struct SimCondition
{
    int engine = 0;
    Preset preset = Preset::RandomCube;
    float timePerFrame = 1000.0f;
    int numberOfParticles = 800;
    float massAvg = 6.0e+29f;
    bool massRandom = true;
};

inline float randf() {return (float)rand() / (float)RAND_MAX;}

// Random numbers in the range of equality between plus and minus areas
inline float rand0centerf()
{
    if (rand() % 2 == 0) {
        return (float)rand();
    } else {
        return -(float)rand();
    }
}

inline float rand0center1maxf()
{
    if (rand() % 2 == 0) {
        return (float)rand() / (float)RAND_MAX;
    } else {
        return -(float)rand() / (float)RAND_MAX;
    }
}

inline QRecursiveMutex interactionMutex;

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
            {1, tr("Gravity 3D Mass Integral (Exp.)")},
            {2, tr("Gravity 3S-Velocity M. Diff.")},
            //{0, tr("Gravity3DIntegral")},
            //{0, tr("Gravity2DDifferential")},
            //{0, tr("Gravity2DIntegral")},
        })
        , PRESET(new QMap<bhs::Preset, QString>{
            {bhs::Preset::RandomCube, tr("Random Cube")},
            {bhs::Preset::RandomSphere, tr("Random Sphere")},
            {bhs::Preset::SunEarth, tr("Sun Earth")},
            {bhs::Preset::EarthMoon, tr("Earth Moon")},
            {bhs::Preset::SunEarthVenus, tr("Sun Earth Venus")},
            {bhs::Preset::EarthSun, tr("Test Earth Sun swap")},
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
    void frameAdvance(int);
    void resultReady();
    void resetParticles(const bhs::SimCondition& sim);
};
