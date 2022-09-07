#pragma once

#include "hnn/quaternion.h"
#include "hnn/biquaternion.h"
#include <QDebug>
#include <QMutex>
#include <QGenericMatrix>

using namespace hnn;

namespace bhs // Black Hole Simulator
{

// Do Not sort or delete for save files
enum class Engine
{
    G3D,
    G3D4D,
    G3D4DR1,
    G4D3D,
    GravityCollision,
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
    Custom,
};

struct Particle
{
    Vector3 coordinate;
    Vector3 velocity;
    double mass;
    QString name;
};

struct CustomData
{
    double scale = 1.0e+14;
    QVector<Particle> particles;
    QString name;
};

struct SimCondition
{
    Engine engine = Engine::G3D;
    Precision precision = Precision::Float;
    Compute compute = Compute::GPU;
    Preset preset = Preset::RandomBall;
    double timePerFrame = 1000.0;

    //##############################################################
    // For Random Preset
    int numberOfParticles = 2000;
    double scale = 1.0e+14;
    double massAvg = 1.0e+37;
    bool massRandom = true;
    double speed = 1.5e+4;
    double rotation = 5e-5;
    //##############################################################

    CustomData custom;
};

struct IntRange
{
    quint64 start;
    quint64 end;
};

enum class LineType
{
    XZMeshes,
    Axis,
    CubeMeshes,
    LongitudeAndLatitude,
    QuaternionS3Rotation,
    OctonionS3RotationXY,
    OctonionS3RotationAll,
};

inline QRecursiveMutex interactionMutex;

double rand0to1();
double rand0center1max();

template <typename T>
void embedQuaternionToArray(const Quaternion& q, T* const a, const quint64 index)
{
    a[index    ] = T(q.re());
    a[index + 1] = T(q.i1());
    a[index + 2] = T(q.i2());
    a[index + 3] = T(q.i3());
}

template <typename T>
void embedVector3ToArray(const Vector3& v, T* const a, const quint64 index)
{
    a[index    ] = T(v.x());
    a[index + 1] = T(v.y());
    a[index + 2] = T(v.z());
}

template <typename T>
void embedMatrix1x4ToArray(const QGenericMatrix<1, 4, double>& m, T* const a, const quint64 index)
{
    a[index    ] = T(m(0, 0));
    a[index + 1] = T(m(1, 0));
    a[index + 2] = T(m(2, 0));
    a[index + 3] = T(m(3, 0));
}

} // namespace

QDebug operator<<(QDebug, const std::complex<double>&);
QDebug operator<<(QDebug, const Biquaternion&);
