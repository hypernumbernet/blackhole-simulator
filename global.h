#pragma once

#include <QObject>
#include <QMutex>

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

enum class Compute
{
    CPU,
    GPU,
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
    Precision precision = Precision::Double;
    Compute compute = Compute::GPU;
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
inline T randf()
{
    return (T)rand() / (T)RAND_MAX;
}

// Random numbers in the range of equality between plus and minus areas
inline float rand0centerf()
{
    if (rand() % 2 == 0)
        return (float)rand();
    else
        return -(float)rand();
}

inline float rand0center1maxf()
{
    if (rand() % 2 == 0)
        return (float)rand() / (float)RAND_MAX;
    else
        return -(float)rand() / (float)RAND_MAX;
}

inline QRecursiveMutex interactionMutex;

}
