#pragma once

#include "hnn/quaternion.h"

#include <QObject>
#include <QMutex>

using namespace hnn;

namespace bhs // Black Hole Simulator
{

enum class Engine
{
    G3D,
    G3D4D,
    G4D3D,
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
    Engine engine = Engine::G3D;
    Precision precision = Precision::Float;
    Compute compute = Compute::GPU;
    Preset preset = Preset::RandomBall;
    double timePerFrame = 1000.0;
    int numberOfParticles = 2000;
    double massAvg = 6.0e+29;
    bool massRandom = true;
    double scale = 1.0e+11;
};

struct IntRange
{
    quint64 start;
    quint64 end;
};

inline double rand0to1()
{
    return (double)rand() / (double)RAND_MAX;
}

// Random numbers in the range of equality between plus and minus areas
inline double rand0center1max()
{
    if (rand() % 2 == 0)
        return (double)rand() / (double)RAND_MAX;
    else
        return -(double)rand() / (double)RAND_MAX;
}

inline QRecursiveMutex interactionMutex;

inline double m_max = 0.0;

template <typename T>
inline void embedQuaternionToArray(const Quaternion<T>& q, T* const a, const quint64 index)
{
    a[index    ] = q.i0;
    a[index + 1] = q.i1;
    a[index + 2] = q.i2;
    a[index + 3] = q.i3;
}

template <typename T>
inline void embedVector3ToArray(const Vector3<T>& v, T* const a, const quint64 index)
{
    a[index    ] = v.x;
    a[index + 1] = v.y;
    a[index + 2] = v.z;
}

}
