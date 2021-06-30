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

enum class Engine
{
    G3DMassDiff,
    G3DMassIntegral,
    G3D4DMassDiff,
};

enum class Precision
{
    Float,
    Double,
};

enum class Preset
{
    RandomCube,
    RandomSphere,
    RandomBall,
    SunEarth,
    EarthSun,
    EarthMoon,
    SunEarthVenus,
    TestSamePosition,
};

struct SimCondition
{
    Engine engine = Engine::G3DMassDiff;
    Precision precision= Precision::Double;
    Preset preset = Preset::RandomCube;
    double timePerFrame = 1000.0;
    int numberOfParticles = 800;
    double massAvg = 6.0e+29;
    bool massRandom = true;
    double scale = 1.0e+11;
};

struct IntRange
{
    quint64 start;
    quint64 end;
};

template <typename T>
inline float randf() {return (T)rand() / (T)RAND_MAX;}

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

    const QMap<bhs::Engine, QString>* const ENGINE;
    const QMap<bhs::Precision, QString>* const PRECISION;
    const QMap<bhs::Preset, QString>* const PRESET;

    static UpdateUi& it()
    {
        static UpdateUi instance;
        return instance;
    }

private:
    UpdateUi()
        : ENGINE(new QMap<bhs::Engine, QString>{
            {bhs::Engine::G3DMassDiff, tr("Gravity 3D Mass Differential")},
            {bhs::Engine::G3DMassIntegral, tr("Gravity 3D Mass Integral (Exp.)")},
            {bhs::Engine::G3D4DMassDiff, tr("Gravity 3S-Velocity M. Diff.")},
            //{0, tr("Gravity3DIntegral")},
            //{0, tr("Gravity2DDifferential")},
            //{0, tr("Gravity2DIntegral")},
        })
        , PRECISION(new QMap<bhs::Precision, QString>{
            {bhs::Precision::Float, tr("Float")},
            {bhs::Precision::Double, tr("Double")},
        })
        , PRESET(new QMap<bhs::Preset, QString>{
            {bhs::Preset::RandomCube, tr("Random Cube")},
            {bhs::Preset::RandomSphere, tr("Random Sphere")},
            {bhs::Preset::RandomBall, tr("Random Ball")},
            {bhs::Preset::SunEarth, tr("Sun Earth")},
            {bhs::Preset::EarthMoon, tr("Earth Moon")},
            {bhs::Preset::SunEarthVenus, tr("Sun Earth Venus")},
            {bhs::Preset::EarthSun, tr("Test Earth Sun swap")},
            {bhs::Preset::TestSamePosition, tr("Test Same Position")},
        })
    {
    };
    UpdateUi(const UpdateUi&);

signals:
    void displayNumberOfParticles(int);
    void displayFrameNumber(int);
    void displayFps(int);
    void displayModelScale(float);
    void displayTimePerFrame(float);
    void updateStartButtonText(bool);
    void frameAdvance(int);
    void resultReady();
    void resetParticles();
    void displayEngineName(bhs::Engine);
    void displayPrecision(bhs::Precision);
    void displayPresetName(bhs::Preset);
};
